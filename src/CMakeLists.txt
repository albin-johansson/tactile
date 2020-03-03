cmake_minimum_required(VERSION 3.12)
project(Tactile2-src)

set(TACTILE_SOURCE_FILES
        tactile2.qrc
        ui/tactile_window.h
        ui/tactile_window.cpp
        core/tile/tile_map.h
        core/tile/tile_map.cpp
        core/tile/tile_id.h
        core/tile/tile_layer.cpp
        core/tile/tile_layer.h
        core/tile/map_position.cpp
        core/tile/map_position.h
        core/util/maybe.h
        core/util/smart_pointers.h
        core/tile/tile_set.cpp
        core/tile/tile_set.h
        core/flood_fill.h
        core/flood_fill.cpp
        editor_pane.cpp
        editor_pane.h
        tactile_application.cpp
        tactile_application.h
        ui/about_dialog.cpp
        ui/about_dialog.h
        ui/settings_dialog.h
        ui/settings_dialog.cpp)

add_library(TactileLib ${TACTILE_SOURCE_FILES})

target_include_directories(TactileLib PUBLIC ${TACTILE_ROOT_DIR}/src)
target_include_directories(TactileLib PUBLIC core)
target_include_directories(TactileLib PUBLIC core/util)
target_include_directories(TactileLib PUBLIC core/tile)
target_include_directories(TactileLib PUBLIC ui)


target_link_libraries(TactileLib
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui)

add_executable(TactileRun
        ${TACTILE_SOURCE_FILES}
        main.cpp)

target_include_directories(TactileRun PUBLIC ${TACTILE_ROOT_DIR}/src)
target_include_directories(TactileRun PUBLIC core)
target_include_directories(TactileRun PUBLIC core/util)
target_include_directories(TactileRun PUBLIC core/tile)
target_include_directories(TactileRun PUBLIC ui)

target_link_libraries(TactileRun
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui)

copy_directory_post_build(TactileRun
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})