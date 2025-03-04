# cosmo-stub-generator

This project uses libclang to parse the header files of any project and output
a stub for use with [cosmopolitan](https://github.com/jart/cosmopolitan).

## Generating Stubs

0. Have lua 5.4
1. Build libclang-lua (make sure to checkout submodules), following the
   instructions at https://github.com/ppebb/libclang-lua.
2. Install [luafilesystem](https://lunarmodules.github.io/luafilesystem/)
3. Add a header file to `./specs` that `#include`s whichever headers you wish
   to process
4. Add a lua file to `./specs` which returns a table in the following format,
   with gobject as an example:
    ```lua
    local utils = require("src.utils")

    return {
        -- The name of the stub, will output in ./stubs/name-stub
        name = "gobject",
        -- A list of filters (written as lua patterns) for the paths containing
        -- your header files. If you don't have these, libclang will traverse
        -- every header in the entire translation unit, and you'll end up with
        -- things like time.h in your stub
        filter = { "%/usr%/include%/glib%-2%.0%/gobject%/", "%/usr%/include%/glib%-2%.0%/glib%-object%.h%/" },
        -- The header file you want to use from step 1, any path. The below will
        -- resolve to ./specs/gobject_spec.h
        hfile = utils.path_combine(utils.spec_path(), "gobject_spec.h"),
        -- The shared object, the name is just used as the variable name internally
        -- for the loaded so, fnames is a list of acceptable library names which will
        -- be searched on program launch
        so = { name = "gobject", fnames = { "libgobject-2.0.so", "libgobject-2.0-0.dll" } },
        -- The directories for libclang to search for headers in, included using
        -- the -isystem argument
        search_dirs = {
            "/usr/include/glib-2.0/",
            "/usr/lib/glib-2.0/include/",
        },
        -- Headers to include in your generated stub
        header_includes = {
            "glib-object.h",
        },
        -- Optional, function bodies you can define to be included in the stub to
        -- overwrite ones automatically generated. Use cases include variadic
        -- functions which do not map correctly to an automatically located function
        -- taking a va_list
        explicit_function_bodies = {
            -- From https://github.com/GNOME/glib/blob/83200855579964a20d3929f37a37431e4952d156/gobject/gobject.c#L2406
            g_object_new = [[gpointer g_object_new(GType object_type, const gchar *first_property_name, ...) {
        GObject *object;
        va_list var_args;

        /* short circuit for calls supplying no properties */
        if (!first_property_name)
            return stub_funcs.ptr_g_object_new_with_properties(object_type, 0, NULL, NULL);

        va_start(var_args, first_property_name);
        object = stub_funcs.ptr_g_object_new_valist(object_type, first_property_name, var_args);
        va_end(var_args);

        return object;
    }]],
            -- From https://github.com/GNOME/glib/blob/7129521966fa7c4cd876b2aa429f1c8d50290902/gobject/gsignal.c#L1399
            g_signal_new = [[guint g_signal_new (const gchar *signal_name, GType itype, GSignalFlags signal_flags, guint class_offset, GSignalAccumulator accumulator, gpointer accu_data, GSignalCMarshaller c_marshaller, GType return_type, guint n_params, ...) {
        va_list args;
        guint signal_id;

        g_return_val_if_fail(signal_name != NULL, 0);
        va_start(args, n_params);

        signal_id = stub_funcs.ptr_g_signal_new_valist(signal_name, itype, signal_flags,
            class_offset ? stub_funcs.ptr_g_signal_type_cclosure_new(itype, class_offset): NULL,
            accumulator, accu_data, c_marshaller, return_type, n_params, args);

        va_end(args);

        return signal_id;
    }]],
        },
    }
    ```
    Additional examples can be located in the `./specs` directory
5. `require` your lua spec in the `stubs` table in `./src/generate.lua`
6. Run `lua src/generate.lua path/to/clang/include`, path/to/clang/include should be somewhere like `/usr/lib/clang/19/include/`

## Included Stubs

This repository includes full[^1] stubs for
* GTK (and GSK, GDK)
* GLib
* GIO
* GObject,
* GModule
* GIRepository
* GLFW
* OpenGL
* SDL2
* SDL2_ttf

[^1]: Some variadic functions may be
missing, but their equivalents should exist.

## Using Stubs

The stubs should work out of the box, just copy the relevant stub directory
(alongside `./stubs/stub.c` and `./stubs/stub.h`) and then include them in your
project. If you don't want 12000 lines of C to appear, add them to
linguist-vendored (see `.gitattributes` at the root of this repository).  

In your main function, make sure you initialize each stub on program launch and
close them on program load. The functions follow the naming convention
`initialize_soname()` and `close_soname()`, using the sonames provided in the
spec.


## Notes

Certain function attributes and keywords may not be handled
currently.`noreturn` functions alongside `volatile` and `restrict` parameters
are examples which are handled. For anything similar that is missing, open up
an issue or a PR.

Additionally, libclang resolves types such as `unsigned int` or `unsigned char`
to `UInt` and `UChar`, which must be manually handled. Refer to the function
`cxtype_name` in `./src/generate.lua`. I have not handled every case, so if you
encounter one not present then you can add another case there (PRs welcome!).
