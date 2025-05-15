cmake_path(SET VCPKG_ROOT NORMALIZE "$ENV{VCPKG_ROOT}")
include("${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
