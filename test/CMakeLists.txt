cmake_minimum_required(VERSION 3.12)
project(tactile_test_proj)

set(TACTILE_TEST_FILES
        unit/test_main.cpp
        unit/tile_layer_test.cpp
        unit/map_position_test.cpp
        unit/tilemap_test.cpp
        unit/flood_fill_test.cpp
        unit/tileset_test.cpp
        unit/tile_size_test.cpp
        unit/widget_size_policy_test.cpp
        unit/tileset_manager_test.cpp)

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