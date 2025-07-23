include_guard(GLOBAL)

set(CMAKE_OSX_SYSROOT "macosx")
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

cmake_path(SET VCPKG_ROOT NORMALIZE "$ENV{VCPKG_ROOT}")
include("${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
