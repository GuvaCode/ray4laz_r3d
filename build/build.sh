#!/bin/bash

rm -f ../libs/x86_64-linux/libr3d*
rm -f ../libs/x86_64-linux/libassimp*
rm -f ../libs/x86_32-linux/libr3d*
rm -f ../libs/x86_32-linux/libassimp*
rm -f ../libs/x86_64-win64/libr3d*
rm -f ../libs/x86_64-win64/libassimp*
rm -f ../libs/i386-win32/libr3d*
rm -f ../libs/i386-win32/libassimp*


git clone https://github.com/raysan5/raylib.git

####     -DRAYLIB_MODULE_RAYGUI=ON \

git clone https://github.com/Bigfoot71/r3d
cd r3d/external

git clone https://github.com/assimp/assimp

cd ../../
cp -r raylib r3d/external

cd r3d

mkdir build_lin64
cd build_lin64

echo " "
echo " -------------------------- "
echo " Build R3D x86_64_LINUX     "
echo " -------------------------- "
echo " "

cmake .. \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DBUILD_SHARED_LIBS=ON \
    -DR3D_BUILD_EXAMPLES=ON \
    -DR3D_RAYLIB_VENDORED=ON \
    -DR3D_ASSIMP_VENDORED=ON \
    -DR3D_BUILD_DOCS=OFF 

cmake --build .

cp libr3d.so ../../../libs/x86_64-linux/libr3d.so
