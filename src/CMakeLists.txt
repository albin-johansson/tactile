cmake_minimum_required(VERSION 3.12)
project(Tactile-src)

function(tactile_include_dirs [target])
    target_include_directories(${ARGV0} PUBLIC ${TACTILE_ROOT_DIR}/src)
    target_include_directories(${ARGV0} PUBLIC core)
    target_include_directories(${ARGV0} PUBLIC core/util)
    target_include_directories(${ARGV0} PUBLIC core/tile)
    target_include_directories(${ARGV0} PUBLIC ui)
    target_include_directories(${ARGV0} PUBLIC ui/toolbar)
endfunction()

function(tactile_append_gcc_compiler_flags [target])
    target_compile_options(${ARGV0} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wdouble-promotion
            -Wswitch-default
            -Wswitch-enum
            -Wunused
            -Wuninitialized
            -Wsuggest-final-types
            -Wsuggest-final-methods
            -Wsuggest-override
            -Wduplicated-cond
            -Wconversion
            -Wc++14-compat
            -Wc++17-compat)
endfunction()

function(tactile_link_libraries [target])
    target_link_libraries(${ARGV0}
            PUBLIC Qt5::Core
            PUBLIC Qt5::Widgets
            PUBLIC Qt5::Gui)
endfunction()

set(TACTILE_SOURCE_FILES
        tactile2.qrc

        core/util/maybe.h
        core/util/smart_pointers.h
        core/util/algorithm_utils.h

        core/tile/tile_map.h
        core/tile/tile_map.cpp
        core/tile/tile_id.h
        core/tile/tile_layer.cpp
        core/tile/tile_layer.h
        core/tile/map_position.cpp
        core/tile/map_position.h
        core/tile/tile_set.cpp
        core/tile/tile_set.h
        core/tile/tile_sheet.cpp
        core/tile/tile_sheet.h
        core/tile/tile_size.h
        core/tile/tile_size.cpp

        core/flood_fill.h
        core/flood_fill.cpp
        core/tactile_editor.h
        core/tactile_editor.cpp

        ui/editor_pane.cpp
        ui/editor_pane.h
        ui/about_dialog.cpp
        ui/about_dialog.h
        ui/settings_dialog.h
        ui/settings_dialog.cpp
        ui/mouse_tool_widget.cpp
        ui/mouse_tool_widget.h
        ui/tactile_window.h
        ui/tactile_window.cpp
        ui/tile_sheet_file_dialog.cpp
        ui/tile_sheet_file_dialog.h
        ui/startup_widget.cpp
        ui/startup_widget.h
        ui/main_editor_widget.cpp 
        ui/main_editor_widget.h
        ui/resize_dialog.h
        ui/resize_dialog.cpp

        ui/toolbar/dynamic_tool_bar_widget.cpp
        ui/toolbar/dynamic_tool_bar_widget.h
        ui/toolbar/stamp_toolbar.cpp
        ui/toolbar/stamp_toolbar.h
        
        tactile_application.cpp
        tactile_application.h core/tile/tile_map_renderer.cpp core/tile/tile_map_renderer.h)

add_library(TactileLib ${TACTILE_SOURCE_FILES})

tactile_include_dirs(TactileLib)
tactile_link_libraries(TactileLib)

add_executable(TactileRun
        ${TACTILE_SOURCE_FILES}
        main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    tactile_append_gcc_compiler_flags(TactileLib)
    tactile_append_gcc_compiler_flags(TactileRun)
endif ()

tactile_include_dirs(TactileRun)
tactile_link_libraries(TactileRun)

copy_directory_post_build(TactileRun
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})