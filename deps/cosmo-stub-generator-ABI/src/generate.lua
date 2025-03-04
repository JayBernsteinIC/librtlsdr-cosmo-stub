local clang = require("libclang-lua")
local lfs = require("lfs")
local utils = require("src.utils")

-- BEGIN!!

local script_path = utils.get_script_dir()
assert(script_path, "Unable to determine script path!")

local stubs_root = utils.path_combine(script_path, "..", "stubs")
if not utils.file_exists(stubs_root) then
    assert(lfs.mkdir(stubs_root))
end

local stubs = {
    require("specs.libusb_spec"),
    nil,
}

--- @param name string
--- @return string
local function cxtype_name(name)
    if name == "Char_S" then
        return "char"
    elseif name == "LongDouble" then
        return "long double"
    elseif name == "UInt" then
        return "unsigned int"
    elseif name == "UChar" then
        return "unsigned char"
    elseif name == "ULong" then
        return "unsigned long"
    elseif name == "UShort" then
        return "unsigned short"
    else
        return name:lower()
    end
end

--- @return string name, integer|nil len, table|nil arguments
local function get_full_type(type, flip)
    local _type = type

    local ptr_str = ""

    while _type:name() == "Pointer" do
        local const_str = _type:is_const() and " const" or ""
        local volatile_str = _type:is_volatile() and " volatile" or ""
        local restrict_str = _type:is_restrict() and " restrict" or ""

        if not flip then
            ptr_str = ptr_str .. const_str .. volatile_str .. " *"
        else
            ptr_str = ptr_str .. " *" .. const_str .. volatile_str .. restrict_str
        end

        _type = _type:pointee()
    end

    if _type:name() == "Elaborated" then
        local struct_str = _type:get_named_type():name() == "Record" and "struct " or ""
        local const_str = _type:is_const() and "const " or ""
        local volatile_str = _type:is_volatile() and "volatile " or ""
        local restrict_str = _type:is_restrict() and "restrict " or ""
        local e_type = _type:declaration()

        return const_str .. volatile_str .. restrict_str .. struct_str .. e_type:name() .. ptr_str, nil
    elseif _type:name() == "ConstantArray" then
        local array_type = _type:get_array_element_type()
        local len = _type:get_array_size()

        return get_full_type(array_type, true), len
    elseif _type:name() == "IncompleteArray" then
        local array_type = _type:get_array_element_type()

        return get_full_type(array_type, true), 0
    elseif _type:name() == "FunctionProto" then
        local result_type = get_full_type(_type:result_type())

        return result_type, nil, _type:arguments()
    else
        local const_str = _type:is_const() and "const " or ""
        local volatile_str = _type:is_volatile() and "volatile " or ""
        local restrict_str = _type:is_restrict() and "restrict " or ""

        return const_str .. volatile_str .. restrict_str .. cxtype_name(_type:name()) .. ptr_str, nil
    end
end

local fmt_args

local function fmt_arg(arg, idx, name_only, ignore_missing_name)
    assert(
        not (name_only and ignore_missing_name),
        "fmt_arg must only have name_only or ignore_missing_name set, not both"
    )

    local name = arg.type and arg:name() or ""

    if not ignore_missing_name and (not name or #name == 0) then
        name = "_" .. tostring(idx)
    end

    if name_only then
        return name
    end

    -- If when passing in fptr_args recursively, it's a list of types and not a
    -- list of cursors
    local type, len, fptr_args = get_full_type(arg.type and arg:type() or arg)
    local array_str = ""
    if len == 0 then
        array_str = "[]"
    elseif len then
        array_str = string.format("[%s]", len)
    end

    if #array_str ~= 0 then
        name = name .. array_str
    end

    if not fptr_args then
        return string.format("%s %s", type, name)
    else
        return string.format("%s (*%s)(%s)", type, name, fmt_args(fptr_args, arg:type():is_variadic(), false, true))
    end
end

fmt_args = function(args, is_var, name_only, ignore_missing_name)
    local ret
    if #args == 0 then
        ret = name_only and "" or "void"
    else
        ret = table.concat(
            utils.transform(args, function(k, v) return fmt_arg(v, k, name_only, ignore_missing_name) end),
            ", "
        )
    end

    if is_var and not name_only then
        ret = ret .. ", ..."
    end

    return ret
end

local function try_find_va_equivalent(funcs, fname, fargs)
    local patterns = {}
    local fsplit = utils.split(fname, "_")

    table.insert(patterns, fname .. "[_]*va*$")
    table.insert(patterns, fname .. "[_]*valist$")

    for i = 1, #fsplit do
        local prefix_vfname = ""

        for j, segment in ipairs(fsplit) do
            prefix_vfname = prefix_vfname .. (j ~= 1 and "_" or "") .. (j == i and "v" or "") .. segment
        end

        table.insert(patterns, prefix_vfname)
    end

    local function print_err(name, args, pattern, err)
        print(
            string.format(
                "Found va_equiv for func %s as func %s(%s) with pattern %s, but err: %s\n",
                fname,
                name,
                fmt_args(args),
                pattern,
                err
            )
        )
    end

    local function check_args(args)
        for _, arg in ipairs(args) do
            if get_full_type(arg:type()):match("va_list") then
                return true
            end
        end

        return false
    end

    for _, func in ipairs(funcs) do
        local name = func:name()
        local args = func:arguments()
        for _, _pattern in ipairs(patterns) do
            if name:match(_pattern) then
                if not check_args(args) then
                    print_err(name, args, _pattern, "va_list missing")
                elseif #args ~= #fargs + 1 then
                    print_err(
                        name,
                        args,
                        _pattern,
                        string.format("args count mismatch, expected: %s, found: %s", #fargs + 1, #args)
                    )
                else
                    return func
                end
            end
        end
    end

    return nil
end

local RAYO_COSMICO = "__builtin_unreachable(); "

--- @return string sig
local function gen_func_signature(name, ret_str, ret_type, args_str, fptr_args)
    -- TODO: What if the function pointer returns a function pointer
    if fptr_args then
        return string.format(
            "%s (*%s(%s))(%s)",
            ret_str,
            name,
            args_str,
            fmt_args(fptr_args, ret_type:is_variadic(), false)
        )
    else
        return string.format("%s (%s)(%s)", ret_str, name, args_str)
    end
end

local function gen_variadic_func(funcs, fname, fargs, ret_type_str, fptr_args)
    local va_equiv = try_find_va_equivalent(funcs, fname, fargs)

    if not va_equiv then
        return nil
    end

    local nonvoid = ret_type_str ~= "void" and not fptr_args
    local ret_var = nonvoid and (ret_type_str .. " ret; ") or ""

    local va_equiv_args = va_equiv:arguments()

    return string.format(
        "{ %sva_list vaargs; va_start(vaargs, %s); %sstub_funcs.ptr_%s(%s); va_end(vaargs); %s}",
        ret_var,
        fargs[#fargs]:name(),
        nonvoid and "ret = " or "",
        va_equiv:name(),
        fmt_args(fargs, true, true)
            .. string.format(", %svaargs", va_equiv_args[#va_equiv_args]:type():name() == "Pointer" and "&" or ""),
        va_equiv:is_no_return() and RAYO_COSMICO or nonvoid and "return ret; " or ""
    )
end

--- return string c struct def, string c load sym, string c function def, string header function def
local function gen_ms_func(funcs, func, soname)
    local name = func:name()
    local ret_type_str, _, fptr_args = get_full_type(func:result_type())
    local args = func:arguments()
    local is_variadic = func:type():is_variadic()

    local args_str = fmt_args(args, is_variadic, false)

    local sig = gen_func_signature(name, ret_type_str, func:result_type(), args_str, fptr_args)

    local struct_def_sig_name
    if fptr_args then
        struct_def_sig_name = string.format("(__attribute__((__ms_abi__)) *ptr_ms_%s)", name)
    else
        struct_def_sig_name = "__attribute__((__ms_abi__)) *ptr_ms_" .. name
    end

    local struct_def_sig =
        gen_func_signature(struct_def_sig_name, ret_type_str, func:result_type(), args_str, fptr_args)

    local success = true
    local func_body
    if is_variadic then
        func_body = gen_variadic_func(funcs, name, args, ret_type_str, fptr_args)

        if not func_body then
            success = false
        end
    else
        local ret_kwd = (ret_type_str == "void" and not fptr_args and "") or "return "
        local args_inner_str = fmt_args(args, false, true)
        func_body = string.format(
            "{ if(IsWindows){} %sstub_funcs.ptr_ms%s(%s); %s}",
            ret_kwd,
            name,
            args_inner_str,
            func:is_no_return() and RAYO_COSMICO or ""
        )
    end

    return string.format("    %s;", struct_def_sig),
        string.format('    stub_funcs.ptr_ms_%s = try_find_sym(%s, "%s");', name, soname, name),
        func_body and string.format("%s %s", sig, func_body) or nil,
        sig .. ";",
        success
end

local function gen_func(funcs, func, soname)
    local name = func:name()
    local ret_type_str, _, fptr_args = get_full_type(func:result_type())
    local args = func:arguments()
    local is_variadic = func:type():is_variadic()

    local args_str = fmt_args(args, is_variadic, false)

    local sig = gen_func_signature(name, ret_type_str, func:result_type(), args_str, fptr_args)

    local struct_def_sig_name
    if fptr_args then
        struct_def_sig_name = string.format("(*ptr_%s)", name)
    else
        struct_def_sig_name = "*ptr_" .. name
    end

    local struct_def_sig =
        gen_func_signature(struct_def_sig_name, ret_type_str, func:result_type(), args_str, fptr_args)

    local success = true
    local func_body
    if is_variadic then
        func_body = gen_variadic_func(funcs, name, args, ret_type_str, fptr_args)

        if not func_body then
            success = false
        end
    else
        local ret_kwd = (ret_type_str == "void" and not fptr_args and "") or "return "
        local args_inner_str = fmt_args(args, false, true)

        func_body = string.format(
            "{ \n\tif((IsLinux())){\n\t\t%sstub_funcs.ptr_%s(%s); %s \n\t}\n\telse {\n\t\t%sstub_funcs.ptr_ms_%s(%s); %s\n\t}}",
            ret_kwd,
            name,
            args_inner_str,
            func:is_no_return() and RAYO_COSMICO or "",
            ret_kwd,
            name,
            args_inner_str,
            func:is_no_return() and RAYO_COSMICO or ""
        )
    end

    return string.format("    %s;", struct_def_sig),
        string.format('    stub_funcs.ptr_%s = try_find_sym(%s, "%s");', name, soname, name),
        func_body and string.format("%s %s", sig, func_body) or nil,
        sig .. ";",
        success
end

for _, stub in ipairs(stubs) do
    print("Generating " .. stub.name)
    local stub_dir = utils.path_combine(stubs_root, stub.name .. "-stub")
    if not utils.file_exists(stub_dir) then
        assert(lfs.mkdir(stub_dir))
    end

    local c_out_path = utils.path_combine(stub_dir, stub.name .. "_stub.c")
    local c_text = {}
    local h_out_path = utils.path_combine(stub_dir, stub.name .. "_stub.h")
    local h_text = {}

    utils.tbl_extend(c_text, {
        string.format('#include "%s_stub.h"', stub.name),
        "#include <stdbool.h>",
        "#include <stdio.h>",
        "#include <stdlib.h>",
        "",
        --"#define _COSMO_SOURCE",
        "#include <libc/dlopen/dlfcn.h>",
        "#define MS_ABI_ATTR __attribute__((__ms_abi__))",
        "",
    })

    assert(arg[1], "Pass your clang include directory as argument 1!")
    local parse_args = { "-isystem", arg[1] }
    for _, dir in ipairs(stub.search_dirs or {}) do
        table.insert(parse_args, "-isystem")
        table.insert(parse_args, dir)
    end
    utils.tbl_extend(parse_args, { stub.hfile })

    local idx = clang.create_index(false, true)
    local tu = assert(idx:parse_translation_unit(parse_args))

    local function find_functions(cursor)
        local ret = {}

        local function find(_cursor)
            for _, c in ipairs(_cursor:children()) do
                if c and c:kind() == "FunctionDecl" and not c:is_inlined() then
                    local loc = c:location() or ""
                    local matched_any = false
                    if loc == stub.hfile then
                        matched_any = true
                    else
                        for _, filter in ipairs(stub.filter) do
                            if loc:find(filter) then
                                matched_any = true
                                break
                            end
                        end
                    end

                    if matched_any then
                        table.insert(ret, c)
                    end
                end

                find(c)
            end
        end

        find(cursor)

        return ret
    end

    local funcs = find_functions(tu:cursor())

    table.insert(c_text, string.format("static struct %sFuncs {", stub.name))

    local c_struct_defs = {}
    local c_load_syms = {}
    local c_func_defs = {}
    local h_func_defs = {}

    for _, func in ipairs(funcs) do
        local c_struct_def, c_load_sym, c_func_def, h_func_def, var_failed = gen_func(funcs, func, stub.so.name)

        local explicit_body = stub.explicit_function_bodies and stub.explicit_function_bodies[func:name()]
        if explicit_body then
            c_func_def = explicit_body
        end

        if c_struct_def and c_load_sym and c_func_def and h_func_def then
            table.insert(c_struct_defs, c_struct_def)
            table.insert(c_load_syms, c_load_sym)
            table.insert(c_func_defs, c_func_def)
            table.insert(h_func_defs, h_func_def)
        elseif var_failed then
            print("skipping matchless va function " .. func:name())
        end
    end

    for _, func in ipairs(funcs) do
        local c_struct_def, c_load_sym, c_func_def, h_func_def, var_failed = gen_ms_func(funcs, func, stub.so.name)

        local explicit_body = stub.explicit_function_bodies and stub.explicit_function_bodies[func:name()]
        if explicit_body then
            c_func_def = explicit_body
        end

        if c_struct_def and c_load_sym and c_func_def and h_func_def then
            table.insert(c_struct_defs, c_struct_def)
            table.insert(c_load_syms, c_load_sym)
        elseif var_failed then
            print("skipping matchless va function " .. func:name())
        end
    end

    utils.tbl_extend(c_text, c_struct_defs)

    utils.tbl_extend(c_text, {
        "} stub_funcs;",
        "",
        string.format("void *%s;", stub.so.name),
        "",
        string.format("void initialize_%s(void) {", stub.name),
        string.format(
            "    char *candidates_%s[] = { %s };",
            stub.so.name,
            table.concat(utils.transform(stub.so.fnames, function(_, v) return string.format('"%s"', v) end), ", ")
        ),
        
        string.format("    %s = try_find_lib(candidates_%s, %s);", stub.so.name, stub.so.name, #stub.so.fnames),
        "",
        string.format("    if (!%s) {", stub.so.name),
        string.format('        fprintf(stderr, "Unable to locate %s, exiting!");', stub.so.name),
        "        exit(1);",
        "    }",
        "",
    })

    utils.tbl_extend(c_text, c_load_syms)
    table.insert(c_text, "}")
    table.insert(c_text, "")

    utils.tbl_extend(c_text, c_func_defs)

    utils.tbl_extend(c_text, {
        "",
        string.format("void close_%s(void) {", stub.name),
        string.format("    cosmo_dlclose(%s);", stub.so.name),
        "}",
    })

    table.insert(h_text, '#include "stub.h" \n #include <cosmo.h>')
    utils.tbl_extend(
        h_text,
        utils.transform(stub.header_includes, function(_, v) return string.format('#include "%s"', v) end)
    )
    utils.tbl_extend(h_text, {
        "",
        string.format("void initialize_%s(void);", stub.name),
        string.format("void close_%s(void);", stub.name),
        "",
    })

    utils.tbl_extend(h_text, h_func_defs)

    utils.file_write(c_out_path, table.concat(c_text, "\n"))
    utils.file_write(h_out_path, table.concat(h_text, "\n"))

    
end
