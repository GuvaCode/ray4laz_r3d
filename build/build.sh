#!/bin/bash

#rm -f ../libs/x86_64-linux/libr3d*

git clone --depth 1 --recurse-submodules https://github.com/Bigfoot71/r3d

cp mingw-w64-x86_64.cmake r3d/mingw-w64-x86_64.cmake

cd r3d

# Очистка предыдущих сборок
rm -rf build_lin64_dynamic build_lin64_static build_win64_dynamic build_win64_static

# ==================== КОМПИЛЯЦИЯ ДИНАМИЧЕСКОЙ БИБЛИОТЕКИ LINUX64 ====================
echo "=== Building dynamic library ==="
mkdir build_lin64_dynamic && cd build_lin64_dynamic
cmake .. -DR3D_RAYLIB_VENDORED=ON -DR3D_ASSIMP_VENDORED=ON -DBUILD_SHARED_LIBS=ON -DR3D_MAX_SHADER_CODE_LENGTH=32768 -DR3D_MAX_SHADER_UNIFORMS=32 -DR3D_MAX_SHADER_SAMPLERS=8 -DR3D_MAX_SCREEN_SHADERS=16 -DR3D_BUILD_EXAMPLES=OFF
cmake --build .
#cp lib/libr3d.so ../../../libs/x86_64-linux/libr3d.so
cp lib/libassimp.so.6.0.2 ../../../libs/x86_64-linux/libassimp.so

cd ..

# ==================== КОМПИЛЯЦИЯ СТАТИЧЕСКОЙ БИБЛИОТЕКИ LINUX64 ====================
echo "=== Building static library ==="
mkdir build_lin64_static && cd build_lin64_static
cmake .. -DR3D_RAYLIB_VENDORED=ON -DR3D_ASSIMP_VENDORED=ON -DBUILD_SHARED_LIBS=OFF -DR3D_MAX_SHADER_CODE_LENGTH=32768 -DR3D_MAX_SHADER_UNIFORMS=32 -DR3D_MAX_SHADER_SAMPLERS=8 -DR3D_MAX_SCREEN_SHADERS=16 -DR3D_BUILD_EXAMPLES=OFF
cmake --build .
cp lib/libr3d.a ../../../libs/x86_64-linux/libr3d.a

cd ..

# ==================== КОМПИЛЯЦИЯ ДИНАМИЧЕСКОЙ БИБЛИОТЕКИ WIN64 ====================
echo "=== Building dynamic library ==="
mkdir build_win64_dynamic && cd build_win64_dynamic
cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake -DR3D_RAYLIB_VENDORED=ON -DR3D_ASSIMP_VENDORED=ON -DBUILD_SHARED_LIBS=ON -DR3D_MAX_SHADER_UNIFORMS=32 -DR3D_MAX_SHADER_SAMPLERS=8 -DR3D_MAX_SCREEN_SHADERS=16 -DR3D_BUILD_EXAMPLES=OFF
cmake --build .
cp bin/libassimp-6.dll ../../../libs/x86_64-win64/libassimp-6.dll
cp bin/libr3d.dll ../../../libs/x86_64-win64/libr3d.dll
cp bin/libraylib.dll ../../../libs/x86_64-win64/libraylib.dll

cd ..
cd ../..
