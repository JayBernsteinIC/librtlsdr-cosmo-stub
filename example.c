#include <stdlib.h>
#include <stdio.h>

#ifdef __COSMOCC__
    #include <cosmo.h>
    #include "deps/libusb_stub.h"
#else
    #include <libusb-1.0/libusb.h>
#endif
#include "include/rtl-sdr.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include <signal.h>
#include <stdatomic.h>
#include <assert.h>



int copy_files_with_prefix(const char *dir, const char *prefix) {
    DIR *dp;
    struct dirent *entry;
    char src_path[512], dest_path[512];
    int fdin, fdout;
    int mode = S_IRUSR | S_IWUSR;  // Read/write permissions for the owner

    // Open the directory
    dp = opendir(dir);
    if (dp == NULL) {
        perror("Failed to open directory");
        return -1; // Error opening directory
    }

    // List filenames in the directory
    //~ printf("Files in directory '%s':\n", dir);
    //~ while ((entry = readdir(dp)) != NULL) {
        //~ if (entry->d_type != DT_DIR) {
            //~ printf("%s\n", entry->d_name);  // Print all filenames
        //~ }
    //~ }
    //~ rewinddir(dp);  // Reset the directory stream for further processing

    // Copy files starting with the prefix "foo"
    while ((entry = readdir(dp)) != NULL) {
        // Skip directories
        if (entry->d_type == DT_DIR) {
            continue;
        }

        // Check if the file starts with the prefix "foo"
        if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
            // Build source and destination file paths
            snprintf(src_path, sizeof(src_path), "%s/%s", dir, entry->d_name);
            snprintf(dest_path, sizeof(dest_path), "./%s", entry->d_name);

            // Check if the destination file already exists
            if (access(dest_path, F_OK) == 0) {
                fprintf(stderr, "File '%s' already exists in the current directory.\n", entry->d_name);
                continue;  // Skip if the file already exists
            }

            // Open the source file for reading
            fdin = open(src_path, O_RDONLY);
            if (fdin == -1) {
                perror(src_path);
                closedir(dp);  // Close the directory
                return -1;     // Error opening source file
            }

            // Create the destination file with specified mode
            fdout = creat(dest_path, mode);
            if (fdout == -1) {
                perror(dest_path);
                close(fdin); // Close the source file
                closedir(dp);  // Close the directory
                return -1;   // Error creating destination file
            }

            // Copy data from source to destination
            if (copyfd(fdin, fdout, -1) == -1) {
                close(fdin);  // Close source file before returning
                close(fdout); // Close destination file before returning
                closedir(dp); // Close the directory
                return -1;    // Error copying data
            }

            // Close the destination file
            if (close(fdout)) {
                perror(dest_path);
                close(fdin);  // Close source file before returning
                closedir(dp); // Close the directory
                return -1;    // Error closing destination file
            }

            // Close the source file
            if (close(fdin)) {
                perror(src_path);
                closedir(dp); // Close the directory
                return -1;    // Error closing source file
            }

            printf("Copied '%s' to current directory.\n", entry->d_name);
        }
    }

    // Close the directory
    closedir(dp);

    return 0; // Success
}


int testlib_extract(const char *zip, const char *to, int mode) { //TODO Jay: Make this pull it into the designated temp directory
    int fdin, fdout;

    // Check if the file already exists
    if (access(to, F_OK) == 0) {
        fprintf(stderr, "File '%s' already exists.\n", to);
        return -2; // File exists, return failure
    }

    // Open the source file for reading
    if ((fdin = open(zip, O_RDONLY)) == -1) {
        perror(zip);
        return -1; // Error opening source file
    }

    // Create the destination file with specified mode
    if ((fdout = creat(to, mode)) == -1) {
        perror(to);
        close(fdin); // Make sure to close the source file
        return -1;   // Error creating destination file
    }

    // Copy data from source to destination
    if (copyfd(fdin, fdout, -1) == -1) {
        close(fdin); // Close source file before returning
        close(fdout); // Close destination file before returning
        return -1;    // Error copying data
    }

    // Close the destination file
    if (close(fdout)) {
        perror(to);
        close(fdin); // Close source file before returning
        return -1;   // Error closing destination file
    }

    // Close the source file
    if (close(fdin)) {
        perror(zip);
        return -1;   // Error closing source file
    }

    return 0; // Success
}

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8]; 

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");
	}
}


int main()
{
	ShowCrashReports();
	
	const char *dir = "/zip";  // Directory to search in
    const char *prefix = "libusb-1.0"; // Prefix to search for

    int result = copy_files_with_prefix(dir, prefix);
    //~ if (result == 0) {
        //~ printf("All matching files copied successfully!\n");
    //~ } else {
        //~ printf("Error moving files.\n");
    //~ }
    initialize_libusb();
    
	const struct libusb_version *ver = libusb_get_version();
    printf("libusb version: %u.%u.%u.%u\n",
           ver->major, ver->minor, ver->micro, ver->nano);
	//NOTE: Very interesting bug. The pointer to ver overflows into parameters of libusb_init. no idea whats up
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		libusb_exit(NULL);
		return (int) cnt;
	}

	print_devs(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	int32_t number_of_sdr = rtlsdr_get_device_count();
	if (number_of_sdr){
		char manufact[256] = {0};
		char product[256] = {0};
		char serial[256] = {0};
		for (int32_t a = 0;a<number_of_sdr;a++){
			rtlsdr_get_device_usb_strings(a,manufact, product, serial);
			printf("Index %d\r\nManufacturer : %s\r\nproduct : %s\r\nserial : %s\r\n",a,manufact, product, serial);
		}
		
	}
	else
		printf("No SDR detected\n");
	
	return 0;
}
