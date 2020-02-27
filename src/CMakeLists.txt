cmake_minimum_required(VERSION 3.12)
project(Tactile2-src)

set(TACTILE_SOURCE_FILES
        tactile_window.h
        tactile_window.cpp
        core/tile_map.h
        core/tile_map.cpp
        core/tile.h
        core/tile.cpp
        core/tile_layer.cpp
        core/tile_layer.h
        core/map_position.cpp
        core/map_position.h
        core/util/maybe.h
        core/util/smart_pointers.h)

add_library(TactileLib ${TACTILE_SOURCE_FILES})

target_include_directories(TactileLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TactileLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_include_directories(TactileLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core/util)

target_link_libraries(TactileLib
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui)

add_executable(TactileRun
        ${TACTILE_SOURCE_FILES}
        main.cpp)

target_link_libraries(TactileRun
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui)

copy_directory_post_build(TactileRun
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})