local utils = require("src.utils")

-- Compute the absolute paths
local libusb_path = utils.path_combine(utils.spec_path(), "../../libusb")
local libusb_spec_path = utils.spec_path()
local libusb_dir_path = utils.path_combine(utils.spec_path(), "./../../libusb/libusb")

-- -- Print out the absolute paths for the relative paths
-- print("Absolute path for libusb: ", libusb_path)
-- print("Absolute path for libusb_spec.h: ", libusb_spec_path)
-- print("Absolute path for ./../libusb/libusb: ", libusb_dir_path)

-- -- List the contents of the directories (use ls -l for detailed info)
-- print("\nListing contents of libusb directory:")
-- os.execute("ls " .. libusb_path)

-- print("\nListing contents of libusb_spec.h (if it's a directory):")
-- os.execute("ls " .. libusb_spec_path)

-- print("\nListing contents of libusb directory (./../libusb/libusb):")
-- os.execute("ls " .. libusb_dir_path)

return {
    name = "libusb",
    filter = { "libusb" }, -- this isn't supposed to be a path, just a snippet from the desired source path. literally a text matching filter
    hfile = utils.path_combine(utils.spec_path(), "libusb_spec.h"),
    so = { name = "libusb", fnames = { "libusb-1.0.so", "libusb-1.0.dll","libusb-1.0.so.0.3.0","libusb-1.0.so.0" } },
    search_dirs = {
        libusb_dir_path, -- Go up two directories from the current directory
        libusb_spec_path,
    },
    header_includes = {
        "libusb.h",
    },
}
