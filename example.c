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

#include <signal.h>
#include <stdatomic.h>
#include <assert.h>

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
	
	if(IsWindows())
	{
		const char src_file[] = "/zip/deps/libusb-1.0.dll";
		const char dst_file[] = "libusb-1.0.dll";

		if (access( src_file, F_OK) == 0) {
        fprintf(stderr, "zipped libusb dll FOUND and can be used as an asset\n");
	}

		if (access("dst_file", F_OK) == 0) { /* File exists */ }
    	int mode = S_IRUSR | S_IWUSR;  // Read and write permissions for the owner

		int result = testlib_extract(src_file, dst_file, mode);
		if (result == 0) {
			printf("dll successfuly unzipped!\n");
		}
		else if(result == -2);
		else {
			printf("dll not unzipped. Manually extract it from the archive (open the .exe with an unzipping tool)\n");
		}
	}
	else if(IsLinux())
	{
		const char src_file[] = "/zip/deps/libusb-1.0.so";
		const char dst_file[] = "./libusb-1.0.so";

		if (access( src_file, F_OK) == 0) {
        fprintf(stderr, "zipped libusb dll FOUND and can be used as an asset\n");
	}

		if (access("dst_file", F_OK) == 0) { /* File exists */ }
    	int mode = S_IRUSR | S_IWUSR;  // Read and write permissions for the owner

		int result = testlib_extract(src_file, dst_file, mode);
		if (result == 0) {
			printf("dll successfuly unzipped!\n");
		}
		else if(result == -2);
		else {
			printf("dll not unzipped. Manually extract it from the archive (open the .exe with an unzipping tool)\n");
		}
	}
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
	
	if (rtlsdr_get_device_count()){
		char* manufact = {0};
		char* product = {0};
		char* serial = {0};
		rtlsdr_get_device_usb_strings(0,manufact, product, serial);
		printf("Manufacturer : %s\r\n, product : %s\r\n, serial : %s\r\n",manufact, product, serial);
		
	}
	else
		printf("No SDR detected\n");
	
	return 0;
}
