#!/bin/bash

rm -f ../libs/x86_64-linux/libr3d*



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
    -DR3D_BUILD_EXAMPLES=OFF \
    -DR3D_RAYLIB_VENDORED=ON \
    -DR3D_ASSIMP_VENDORED=ON \
    -DR3D_BUILD_DOCS=OFF 

cmake --build .

cp libr3d.so ../../../libs/x86_64-linux/libr3d.so
cd ../../



echo " "
echo " -------------------------- "
echo " Build R3D x86_64_WINDOWS   "
echo " -------------------------- "
echo " "

cp mingw-w64-x86_64.cmake r3d/mingw-w64-x86_64.cmake

cd r3d
mkdir build_win64
cd build_win64

cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake \
    -DCMAKE_PREFIX_PATH=$(pwd)/../external/raylib \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DBUILD_SHARED_LIBS=ON \
    -DR3D_BUILD_EXAMPLES=OFF \
    -DR3D_RAYLIB_VENDORED=ON \
    -DR3D_ASSIMP_VENDORED=ON \
    -DR3D_BUILD_DOCS=OFF 


cmake --build .

cp libr3d.dll ../../../libs/x86_64-windows/libr3d.dll





