cmake_minimum_required(VERSION 3.12)
project(tactile_test_proj)

set(TACTILE_TEST_FILES
        test_main.cpp
        tile_layer_test.cpp
        map_position_test.cpp
        tilemap_test.cpp
        flood_fill_test.cpp
        tileset_test.cpp
        tile_size_test.cpp
        widget_size_policy_test.cpp
        tileset_manager_test.cpp)

add_executable(testTactile ${TACTILE_TEST_FILES})

add_library(libCatch INTERFACE)
target_include_directories(libCatch INTERFACE catch)

target_include_directories(testTactile
        PUBLIC ${TACTILE_ROOT_DIR}/src
        PUBLIC ${TACTILE_ROOT_DIR}/src/core
        PUBLIC ${TACTILE_ROOT_DIR}/src/core/util
        PUBLIC ${TACTILE_ROOT_DIR}/src/core/tile)

target_link_libraries(testTactile
        PUBLIC libTactile
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui
        PUBLIC libCatch)

copy_directory_post_build(testTactile
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})

copy_directory_post_build(testTactile
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR})