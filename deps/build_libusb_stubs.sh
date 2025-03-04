#!/bin/sh -e
# requires package libudev-dev, silently fails otherwise
STARTPATH=$PWD
read -p "Do you want to rebuild libusb? (y/n): " response; if [ "$response" = "y" ]; then
	cat $PWD/../libusb_macros.h
	cd libusb
	./autogen.sh
	mkdir build -p
	rm -rf ./build/*
	./configure --prefix=$PWD/build CFLAGS="-g3 -ggdb3 -O0 -include $PWD/../../libusb_macros.h" 
	make -j8
	make install
	mkdir ../linux_build || true
	rm -rf ../linux_build/*
	#~ mkdir ../linux_build/objects || true
	#~ mkdir ../linux_build/examples || true
	cp -r build/* ../linux_build


	mkdir build-win -p
	rm -rf ./build-win/*
	CC=x86_64-w64-mingw32-gcc ./configure --host=x86_64-w64-mingw32 --prefix="$PWD/build-win" CFLAGS="-g3 -ggdb3 -O0 -include $PWD/../../libusb_macros.h"
	make -j8
	make install
	echo "Requires libudev-dev"
	mkdir ../win_build || true
	rm -rf ../win_build/*
	#~ mkdir ../win_build/objects || true
	#~ mkdir ../win_build/examples || true
	cp -r build-win/* ../win_build
	#~ cp examples/.libs/* ../win_build/examples
	#~ cp ../win_build/objects/*.dll ..

	echo "done"
	cp libusb/libusb.h ..


	cd ..
fi

cp libusb.h cosmo-stub-generator-ABI/specs
cp ../libusb_macros.h cosmo-stub-generator-ABI/specs

luarocks-5.4 install luafilesystem --local
cd cosmo-stub-generator-ABI/libclang-lua/

mkdir build || true
rm -rf build/*
cd build
cmake ..
make -j8
cd ../..
cp libclang-lua/build/libclang-lua.so .



echo "If you don't want to use clang + llvm 20, you have to manually modify the cmake of libclang as well"
lua5.4 src/generate.lua /usr/lib/clang/20/include/ 

cp stubs/libusb-stub/libusb* ../
cd ..
echo "done generating stubs. If you rebuilt the dynamic libraries, you will need to manually move them around"
