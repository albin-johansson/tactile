project(tactile-vulkan-renderer CXX)

find_package(Vulkan REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
