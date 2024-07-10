project(tactile-opengl-renderer CXX)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
