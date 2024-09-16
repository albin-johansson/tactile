project(tactile-runtime CXX)

find_package(argparse CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_subdirectory("lib")
