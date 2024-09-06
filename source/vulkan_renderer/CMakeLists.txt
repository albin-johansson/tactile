project(tactile-vulkan-renderer CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
