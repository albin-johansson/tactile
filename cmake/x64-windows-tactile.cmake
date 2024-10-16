set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

if (PORT MATCHES "sdl2|sdl2-image")
  set(VCPKG_LIBRARY_LINKAGE dynamic)
endif ()

message("${PORT} is using ${VCPKG_LIBRARY_LINKAGE} linkage")

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
