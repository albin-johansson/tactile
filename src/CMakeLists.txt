cmake_minimum_required(VERSION 3.12)
project(tactile_src_proj)

set(TACTILE_SOURCE_FILES
        tactile2.qrc

        tactile_application.cpp
        tactile_application.h
        settings_utils.h
        settings_utils.cpp

        core/flood_fill.h
        core/flood_fill.cpp
        core/tactile_core.h
        core/tactile_core.cpp

        core/util/maybe.h
        core/util/smart_pointers.h
        core/util/algorithm_utils.h
        core/util/tactile_types.h
        core/util/type_utils.h

        core/tile/tile_id.h
        core/tile/tile_size.h
        core/tile/tile_size.cpp

        core/tileset/tileset.cpp
        core/tileset/tileset.h
        core/tileset/tileset_manager.cpp
        core/tileset/tileset_manager.h
        core/tileset/tileset_info.h

        core/tilemap/tilemap.h
        core/tilemap/tilemap.cpp
        core/tilemap/tile_layer.cpp
        core/tilemap/tile_layer.h
        core/tilemap/map_position.cpp
        core/tilemap/map_position.h
        core/tilemap/tilemap_renderer.cpp
        core/tilemap/tilemap_renderer.h

        ui/startup_widget.cpp
        ui/startup_widget.h
        ui/startup_widget.ui

        ui/tools/mouse_tool_widget.cpp
        ui/tools/mouse_tool_widget.h
        ui/tools/tool_button_widget.ui

        ui/util/basic_widget_macro.h
        ui/util/widget_size_policy.h
        ui/util/widget_size_policy.cpp

        ui/window/tactile_window.h
        ui/window/tactile_window.cpp
        ui/window/window.ui
        ui/window/create_dock_widget.h
        ui/window/create_dock_widget.cpp

        ui/tileset/tileset_widget.cpp
        ui/tileset/tileset_widget.h
        ui/tileset/tileset_widget.ui
        ui/tileset/tileset_dialog.cpp
        ui/tileset/tileset_dialog.h
        ui/tileset/tileset_dialog.ui
        ui/tileset/tileset_content_page.ui
        ui/tileset/tileset_empty_page.ui
        ui/tileset/open_tileset_image.cpp
        ui/tileset/open_tileset_image.h
        ui/tileset/tileset_empty_page.cpp
        ui/tileset/tileset_empty_page.h
        ui/tileset/tileset_content_page.cpp
        ui/tileset/tileset_content_page.h
        ui/tileset/tileset_image_widget.cpp
        ui/tileset/tileset_image_widget.h
        ui/tileset/tileset_tab.cpp
        ui/tileset/tileset_tab.h
        ui/tileset/tileset_image_label.cpp
        ui/tileset/tileset_image_label.h

        ui/modal/about_dialog.cpp
        ui/modal/about_dialog.h
        ui/modal/settings_dialog.h
        ui/modal/settings_dialog.cpp
        ui/modal/resize_dialog.h
        ui/modal/resize_dialog.cpp
        ui/modal/about.ui
        ui/modal/resize_dialog.ui
        ui/modal/settings.ui

        ui/toolbar/dynamic_tool_bar_widget.cpp
        ui/toolbar/dynamic_tool_bar_widget.h
        ui/toolbar/stamp_toolbar.cpp
        ui/toolbar/stamp_toolbar.h
        ui/toolbar/toolbar_stamp.ui

        ui/editor/editor_tab.cpp
        ui/editor/editor_tab.h
        ui/editor/central_editor_widget.cpp
        ui/editor/central_editor_widget.h
        ui/editor/tile_map_tab_widget.cpp
        ui/editor/tile_map_tab_widget.h
        ui/editor/central_widget.ui
        ui/editor/editor_pane.ui)

add_library(libTactile ${TACTILE_SOURCE_FILES})

target_include_directories(libTactile
        PUBLIC ${TACTILE_ROOT_DIR}/src
        PUBLIC core
        PUBLIC core/util
        PUBLIC core/tile
        PUBLIC core/tilemap
        PUBLIC core/tileset
        PUBLIC ui
        PUBLIC ui/toolbar
        PUBLIC ui/editor
        PUBLIC ui/modal
        PUBLIC ui/tileset
        PUBLIC ui/window
        PUBLIC ui/util
        PUBLIC ui/tools)

target_link_libraries(libTactile
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(libTactile PRIVATE
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
            -Wc++17-compat)
else ()
    target_compile_options(libTactile PRIVATE
            /EHsc
            /MP
            /W3)
endif ()

add_executable(Tactile
        ${TACTILE_SOURCE_FILES}
        main.cpp)

add_dependencies(Tactile libTactile)

target_link_libraries(Tactile
        PUBLIC libTactile)

copy_directory_post_build(Tactile
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})