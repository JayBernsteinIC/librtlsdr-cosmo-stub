#!/bin/sh -e
# sudo sh -c 'echo -1 > /proc/sys/fs/binfmt_misc/WSLInterop'
BASE_DIR=$PWD



if ! command -v cosmocc > /dev/null; then
  export PATH=$PATH:/home/$USER/cosmocc/bin
  if ! command -v cosmocc > /dev/null; then
    echo "You need to add the cosmopolitan toolchain to your PATH."
    exit 1
  fi
fi

COSMO_HOME=$(dirname "$(dirname "$(which cosmocc)")")

rm -rf ./.build || true
rm -rf ./bin || true
mkdir -p .build bin

FLAGS="-g -ggdb -mcosmo -mdbg -Wall -I /deps/libusb.h "
LINUX_FLAGS="-lusb-1.0"

echo $BASE_DIR


read -p "Do you want to rebuild the stubs? (y/n): " response; if [ "$response" = "y" ]; then
	cd deps
	./build_libusb_stubs.sh
	cd ..
	

fi

	# 1) Compile stubs to object files (no linking yet, so no -lusb-1.0 needed here)
	# cosmocc -g -ggdb -mcosmo -Wall -Werror -c stub/stub.c        -o .build/stub.o
	cosmocc $FLAGS -c deps/stub.c        -o .build/stub.o

	cosmocc $FLAGS -c deps/libusb_stub.c -o .build/libusb_stub.o 

	# Compile all the source files into object files
	cosmocc $FLAGS -I include -c src/librtlsdr.c -o .build/librtlsdr.o
	cosmocc $FLAGS -I include -c src/tuner_e4k.c -o .build/tuner_e4k.o
	cosmocc $FLAGS -I include -c src/tuner_fc0012.c -o .build/tuner_fc0012.o
	cosmocc $FLAGS -I include -c src/tuner_fc0013.c -o .build/tuner_fc0013.o
	cosmocc $FLAGS -I include -c src/tuner_fc2580.c -o .build/tuner_fc2580.o
	cosmocc $FLAGS -I include -c src/tuner_r82xx.c -o .build/tuner_r82xx.o

objects=$(find .build -name '*.o' -not -path '*.aarch64/*')
# 2) Link the final binary with the stubs (now we add -lusb-1.0 if on Linux)

cosmocc $FLAGS example.c -o bin/rtl-sdr-test $objects

./zip ./bin/rtl-sdr-test ./deps/libusb-1.0.dll
./zip ./bin/rtl-sdr-test ./deps/libusb-1.0.so

#~ ./zip -r USBATTEMPT bin/ build.sh buildnote deps/ libusb_macros.h libusbtest.c stub/ zip -q
export LD_LIBRARY_PATH=$BASE_DR/bin:$LD_LIBRARY_PATH
cd bin 
cp rtl-sdr-test rtl-sdr-test.exe
./rtl-sdr-test
cd ..
# x86_64-unknown-cosmo-cc $FLAGS libusbtest.c -o bin/libusbtest 
# gcc libusbtest.c -o bin/libusbtest -lusb-1.0

