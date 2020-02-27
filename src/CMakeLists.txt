cmake_minimum_required(VERSION 3.12)
project(Tactile2-src)

set(TACTILE_SOURCE_FILES
        tactile_window.h
        tactile_window.cpp
        core/tile/tile_map.h
        core/tile/tile_map.cpp
        core/tile/tile.h
        core/tile/tile.cpp
        core/tile/tile_layer.cpp
        core/tile/tile_layer.h
        core/tile/map_position.cpp
        core/tile/map_position.h
        core/util/maybe.h
        core/util/smart_pointers.h
        )

add_library(TactileLib ${TACTILE_SOURCE_FILES})

target_include_directories(TactileLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TactileLib PUBLIC core)
target_include_directories(TactileLib PUBLIC core/util)

target_link_libraries(TactileLib
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui)

add_executable(TactileRun
        ${TACTILE_SOURCE_FILES}
        main.cpp)

target_include_directories(TactileRun PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TactileRun PUBLIC core)
target_include_directories(TactileRun PUBLIC core/util)

target_link_libraries(TactileRun
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui)

copy_directory_post_build(TactileRun
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})