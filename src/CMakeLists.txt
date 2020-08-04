cmake_minimum_required(VERSION 3.12)
project(tactile_src_proj)

set(TACTILE_SOURCE_FILES
        tactile.qrc

        app/app.cpp
        app/app.hpp
        app/app_connections.hpp
        app/app_connections.cpp
        app/set_style_sheet.hpp
        app/set_style_sheet.cpp
        app/setup_app.hpp
        app/setup_app.cpp

        misc/version.hpp
        misc/settings_utils.hpp
        misc/settings_utils.cpp
        misc/tactile_error.hpp
        misc/tactile_error.cpp
        misc/tactile_types.hpp

        model/core_fwd.hpp
        model/core.hpp
        model/core.cpp
        model/flood_fill.hpp
        model/flood_fill.cpp

        model/util/algorithm_utils.hpp
        model/util/type_utils.hpp
        model/tile/tile_size.hpp
        model/tile/tile_size.cpp

        model/tileset/tileset.cpp
        model/tileset/tileset.hpp
        model/tileset/tileset_manager.cpp
        model/tileset/tileset_manager.hpp
        model/tileset/tileset_info.hpp

        model/tilemap/tilemap.hpp
        model/tilemap/tilemap.cpp
        model/tilemap/tile_layer.cpp
        model/tilemap/tile_layer.hpp
        model/tilemap/map_position.cpp
        model/tilemap/map_position.hpp
        model/tilemap/render_tilemap.cpp
        model/tilemap/render_tilemap.hpp

        ui/startup/startup_widget.cpp
        ui/startup/startup_widget.hpp
        ui/startup/startup_widget.ui

        ui/tools/mouse_tool_widget.cpp
        ui/tools/mouse_tool_widget.hpp
        ui/tools/tool_button_widget.ui

        ui/util/basic_widget_macro.hpp
        ui/util/widget_size_policy.hpp
        ui/util/widget_size_policy.cpp

        ui/window/window.hpp
        ui/window/window.cpp
        ui/window/window.ui
        ui/window/create_dock_widget.hpp
        ui/window/create_dock_widget.cpp

        ui/tileset/tileset_widget.cpp
        ui/tileset/tileset_widget.hpp
        ui/tileset/tileset_widget.ui
        ui/tileset/tileset_dialog.cpp
        ui/tileset/tileset_dialog.hpp
        ui/tileset/tileset_dialog.ui
        ui/tileset/tileset_content_page.ui
        ui/tileset/tileset_empty_page.ui
        ui/tileset/open_tileset_image.cpp
        ui/tileset/open_tileset_image.hpp
        ui/tileset/tileset_empty_page.cpp
        ui/tileset/tileset_empty_page.hpp
        ui/tileset/tileset_content_page.cpp
        ui/tileset/tileset_content_page.hpp
        ui/tileset/tileset_image_widget.cpp
        ui/tileset/tileset_image_widget.hpp
        ui/tileset/tileset_tab.cpp
        ui/tileset/tileset_tab.hpp
        ui/tileset/tileset_image_label.cpp
        ui/tileset/tileset_image_label.hpp

        ui/modal/about_dialog.cpp
        ui/modal/about_dialog.hpp
        ui/modal/settings_dialog.hpp
        ui/modal/settings_dialog.cpp
        ui/modal/resize_dialog.hpp
        ui/modal/resize_dialog.cpp
        ui/modal/about.ui
        ui/modal/resize_dialog.ui
        ui/modal/settings.ui

        ui/toolbar/dynamic_tool_bar_widget.cpp
        ui/toolbar/dynamic_tool_bar_widget.hpp
        ui/toolbar/stamp_toolbar.cpp
        ui/toolbar/stamp_toolbar.hpp
        ui/toolbar/toolbar_stamp.ui

        ui/editor/tilemap_scene.cpp
        ui/editor/tilemap_scene.hpp
        ui/editor/central_editor_widget.cpp
        ui/editor/central_editor_widget.hpp
        ui/editor/tilemap_tab.cpp
        ui/editor/tilemap_tab.hpp
        ui/editor/central_widget.ui
        ui/editor/editor_pane.ui misc/tactile_fwd.hpp

        command/command_stack.cpp
        command/command_stack.hpp
        command/add_row.cpp
        command/add_row.hpp
        command/add_col.hpp
        command/add_col.cpp
        command/remove_row.hpp
        command/remove_row.cpp
        command/remove_col.hpp
        command/remove_col.cpp
        command/resize_map.cpp
        command/resize_map.hpp)

add_library(libTactile ${TACTILE_SOURCE_FILES})

target_include_directories(libTactile
        PUBLIC ${TACTILE_ROOT_DIR}/src
        PUBLIC command
        PUBLIC app
        PUBLIC misc
        PUBLIC model
        PUBLIC model/util
        PUBLIC model/tile
        PUBLIC model/tilemap
        PUBLIC model/tileset
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