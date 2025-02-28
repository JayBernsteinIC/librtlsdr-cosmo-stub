# requires package libudev-dev, silently fails otherwise
STARTPATH=$PWD
read -p "Do you want to rebuild libusb? (y/n): " response; if [ "$response" = "y" ]; then
	cd libusb
	./autogen.sh

	cp libusb/libusb.h ..
	echo "Requires libudev-dev"
	make clean
	HEREPATH=$PWD
	./configure  "CC=x86_64-w64-mingw32-gcc" --host=x86_64-w64-mingw32 --prefix=$STARTPATH/libusb --enable-udev --enable-examples-build --verbose
	make -j6
	mkdir ../win_build || true
	mkdir ../win_build/objects || true
	mkdir ../win_build/examples || true
	cp libusb/.libs/* ../win_build/objects
	cp examples/.libs/* ../win_build/examples
	cp ../win_build/objects/*.dll ..

	make clean 
	./configure "CC=gcc" --prefix=$STARTPATH/libusb --enable-udev --enable-examples-build --verbose
	make -j6
	mkdir ../linux_build || true
	mkdir ../linux_build/objects || true
	mkdir ../linux_build/examples || true
	cp libusb/.libs/* ../linux_build/objects
	cp examples/.libs/* ../linux_build/examples
	cp ../linux_build/objects/*.so ..

	cd ..
fi
echo "If building libusb_spec.lua is not in generate.lua in cosmo_stub_generator, this will not work"
rm -rf cosmo-stub-generator-ABI/specs/*
cp localcopy/libusb_spec.* cosmo-stub-generator-ABI/specs/
cp localcopy/libclangs_fixed_CMakeLists.txt  cosmo-stub-generator-ABI/libclang-lua/CMakeLists.txt
cp libusb.h cosmo-stub-generator-ABI/specs/


luarocks-5.4 install luafilesystem --local
cd cosmo-stub-generator-ABI/libclang-lua/

mkdir build || true
cd build
cmake ..
make -j6
cd ../..
cp libclang-lua/build/libclang-lua.so .

echo "If you don't want to use clang + llvm 20, you have to manually modify the cmake of libclang as well"
lua5.4 src/generate.lua /usr/lib/clang/20/include/ 

cp stubs/libusb-stub/libusb* ../
cd ..
echo "done, if generate.lua is modified with the right spec file"
