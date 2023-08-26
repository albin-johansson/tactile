project(tactile-core)

set(TACTILE_CORE_DIR "${TACTILE_ROOT_DIR}/modules/core")

add_subdirectory(src)

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()