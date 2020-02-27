cmake_minimum_required(VERSION 3.12)
project(Tactile2-test)

set(TACTILE_TEST_FILES
        test_main.cpp
        tile_layer_test.cpp
        map_position_test.cpp
        tile_map_test.cpp)

add_executable(TactileTests ${TACTILE_TEST_FILES})

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${TACTILE_LIB_DIR}/catch2)

target_include_directories(TactileTests PUBLIC ${TACTILE_ROOT_DIR}/src)
target_include_directories(TactileTests PUBLIC ${TACTILE_ROOT_DIR}/src/core)
target_include_directories(TactileTests PUBLIC ${TACTILE_ROOT_DIR}/src/core/util)
target_include_directories(TactileTests PUBLIC ${TACTILE_ROOT_DIR}/src/core/tile)

target_link_libraries(TactileTests
        PUBLIC TactileLib
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui
        Catch2)

copy_directory_post_build(TactileTests
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})