project(tactile-runtime CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_subdirectory("lib")
