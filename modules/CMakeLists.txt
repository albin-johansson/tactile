project(tactile-modules CXX)

if (TACTILE_BUILD_TESTS MATCHES ON)
  find_package(GTest CONFIG REQUIRED)
endif ()

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

add_subdirectory(foundation)

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(testutil)
endif ()

add_subdirectory(proto)
add_subdirectory(tactile-yml-format)
add_subdirectory(tiled-xml-format)
add_subdirectory(tiled-json-format)
add_subdirectory(godot-scene-format)
add_subdirectory(core)

if (TACTILE_BUILD_OPENGL_RENDERER MATCHES ON)
  add_subdirectory(opengl-renderer)
endif ()

if (TACTILE_BUILD_VULKAN_RENDERER MATCHES ON)
  add_subdirectory(vulkan-renderer)
endif ()

add_subdirectory(main)
