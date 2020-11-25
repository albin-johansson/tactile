cmake_minimum_required(VERSION 3.12)

project(tactile_src CXX)

set(TACTILE_SOURCE_FILES
        tactile.qrc

        tactile_app.cpp
        tactile_app.hpp
        version.hpp

        core/model.hpp
        core/model.cpp
        core/tactile_error.hpp
        core/position.hpp

        core/aliases/czstring.hpp
        core/aliases/tile_id.hpp
        core/aliases/tile_width.hpp
        core/aliases/tile_height.hpp
        core/aliases/layer_id.hpp
        core/aliases/map_id.hpp
        core/aliases/tileset_id.hpp
        core/aliases/maybe.hpp

        core/algorithm/flood_fill.hpp
        core/algorithm/flood_fill.cpp
        core/algorithm/algorithm.hpp

        core/tile/tile_size.hpp
        core/tile/tile_size.cpp

        core/tileset/tileset.cpp
        core/tileset/tileset.hpp
        core/tileset/tileset_manager.cpp
        core/tileset/tileset_manager.hpp

        core/map/map.hpp
        core/map/map.cpp
        core/map/layer.cpp
        core/map/layer.hpp
        core/map/map_document.cpp
        core/map/map_document.hpp
        core/map/map_manager.cpp
        core/map/map_manager.hpp

        gui/icons.cpp
        gui/icons.hpp
        gui/dock_widget.cpp
        gui/dock_widget.hpp
        gui/tab_bar.cpp
        gui/tab_bar.hpp
        gui/tab_widget.cpp
        gui/tab_widget.hpp

        gui/startup/startup_widget.cpp
        gui/startup/startup_widget.hpp
        gui/startup/startup_widget.ui

        gui/tools/tool_widget.cpp
        gui/tools/tool_widget.hpp
        gui/tools/tool_widget.ui
        gui/tools/tool_dock.cpp
        gui/tools/tool_dock.hpp

        gui/window/window.hpp
        gui/window/window.cpp
        gui/window/window.ui

        gui/tileset/tileset_widget.cpp
        gui/tileset/tileset_widget.hpp
        gui/tileset/tileset_widget.ui
        gui/tileset/tileset_content_page.ui
        gui/tileset/tileset_empty_page.ui
        gui/tileset/tileset_empty_page.cpp
        gui/tileset/tileset_empty_page.hpp
        gui/tileset/tileset_content_page.cpp
        gui/tileset/tileset_content_page.hpp
        gui/tileset/tileset_image_widget.cpp
        gui/tileset/tileset_image_widget.hpp
        gui/tileset/tileset_tab.cpp
        gui/tileset/tileset_tab.hpp
        gui/tileset/tileset_image_label.cpp
        gui/tileset/tileset_image_label.hpp
        gui/tileset/tileset_dock.cpp
        gui/tileset/tileset_dock.hpp
        gui/tileset/tileset_tab_manager.cpp
        gui/tileset/tileset_tab_manager.hpp
        gui/tileset/tileset_tab_context_menu.cpp
        gui/tileset/tileset_tab_context_menu.hpp
        gui/tileset/tileset_rubber_band.cpp
        gui/tileset/tileset_rubber_band.hpp

        gui/layer/layer_dock.cpp
        gui/layer/layer_dock.hpp
        gui/layer/layer_widget.cpp
        gui/layer/layer_widget.hpp
        gui/layer/layer_widget.ui
        gui/layer/layer_item.cpp
        gui/layer/layer_item.hpp
        gui/layer/layer_item_context_menu.cpp
        gui/layer/layer_item_context_menu.hpp

        gui/settings/settings_dialog.hpp
        gui/settings/settings_dialog.cpp
        gui/settings/settings_dialog.ui

        gui/dialog/about/about_dialog.cpp
        gui/dialog/about/about_dialog.hpp
        gui/dialog/about/about_dialog.ui

        gui/dialog/resize_map/resize_dialog.hpp
        gui/dialog/resize_map/resize_dialog.cpp
        gui/dialog/resize_map/resize_dialog.ui

        gui/dialog/save/save_as_dialog.cpp
        gui/dialog/save/save_as_dialog.hpp

        gui/dialog/open_map/open_map_dialog.cpp
        gui/dialog/open_map/open_map_dialog.hpp
        gui/dialog/open_map/open_map_error_dialog.ui
        gui/dialog/open_map/open_map_error_dialog.cpp
        gui/dialog/open_map/open_map_error_dialog.hpp

        gui/dialog/open_tileset/tileset_dialog.cpp
        gui/dialog/open_tileset/tileset_dialog.hpp
        gui/dialog/open_tileset/tileset_dialog.ui
        gui/dialog/open_tileset/open_tileset_image.cpp
        gui/dialog/open_tileset/open_tileset_image.hpp

        gui/toolbar/dynamic_tool_bar_widget.cpp
        gui/toolbar/dynamic_tool_bar_widget.hpp
        gui/toolbar/stamp_toolbar.cpp
        gui/toolbar/stamp_toolbar.hpp
        gui/toolbar/toolbar_stamp.ui

        gui/editor/map_scene.cpp
        gui/editor/map_scene.hpp
        gui/editor/map_editor.cpp
        gui/editor/map_editor.hpp
        gui/editor/map_tab_widget.cpp
        gui/editor/map_tab_widget.hpp
        gui/editor/map_view.cpp
        gui/editor/map_view.hpp
        gui/editor/map_item.cpp
        gui/editor/map_item.hpp
        gui/editor/map_editor.ui

        command/command_id.hpp
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
        command/resize_map.hpp
        command/bucket_fill.cpp
        command/bucket_fill.hpp
        command/stamp_sequence.cpp
        command/stamp_sequence.hpp
        command/erase_sequence.cpp
        command/erase_sequence.hpp
        command/repeated_command.cpp
        command/repeated_command.hpp
        command/add_tileset.cpp
        command/add_tileset.hpp
        command/remove_tileset.cpp
        command/remove_tileset.hpp
        command/repeated_map_command.hpp
        command/repeated_map_command.cpp
        command/map_command.cpp
        command/map_command.hpp
        command/remove_layer.cpp
        command/remove_layer.hpp
        command/add_layer.cpp
        command/add_layer.hpp

        prefs/setting.hpp
        prefs/theme.cpp
        prefs/theme.hpp
        prefs/preferences.cpp
        prefs/preferences.hpp

        parse/parse_palette.cpp
        parse/parse_palette.hpp
        parse/xml_utils.hpp
        parse/xml_utils.cpp
        parse/json_utils.cpp
        parse/json_utils.hpp

        parse/tmx/parse_error.hpp
        parse/tmx/parse_error.cpp
        parse/tmx/map_file_type.hpp
        parse/tmx/element_id.hpp
        parse/tmx/map_parser.hpp
        parse/tmx/json_element.cpp
        parse/tmx/json_element.hpp
        parse/tmx/xml_element.cpp
        parse/tmx/xml_element.hpp
        parse/tmx/xml_engine.cpp
        parse/tmx/xml_engine.hpp
        parse/tmx/json_engine.cpp
        parse/tmx/json_engine.hpp

        tool/tool_id.hpp
        tool/tool_model.cpp
        tool/tool_model.hpp
        tool/abstract_tool.hpp
        tool/abstract_tool.cpp
        tool/stamp_tool.cpp
        tool/stamp_tool.hpp
        tool/eraser_tool.hpp
        tool/eraser_tool.cpp
        tool/bucket_tool.cpp
        tool/bucket_tool.hpp

        util/nenya.hpp
        util/vector_map.hpp
        util/set_style_sheet.hpp
        util/set_style_sheet.cpp
        util/setup_app.hpp
        util/setup_app.cpp
        util/signal_blocker.hpp

        service/save/save_tmx.cpp
        service/save/save_tmx.hpp
        service/save/save_service.cpp
        service/save/save_service.hpp
        service/save/save_json.cpp
        service/save/save_json.hpp
        service/save/export_options.hpp
        service/save/tiled_version.hpp

        service/open/open_map.cpp
        service/open/open_map.hpp)

add_library(libTactile ${TACTILE_SOURCE_FILES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif ()

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

target_include_directories(libTactile
        PUBLIC ${TACTILE_ROOT_DIR}/src
        PUBLIC command
        PUBLIC prefs
        PUBLIC parse
        PUBLIC parse/tmx
        PUBLIC tool
        PUBLIC util
        PUBLIC core
        PUBLIC core/util
        PUBLIC core/tile
        PUBLIC core/map
        PUBLIC core/tileset
        PUBLIC core/algorithm
        PUBLIC core/aliases
        PUBLIC gui
        PUBLIC gui/startup
        PUBLIC gui/toolbar
        PUBLIC gui/editor
        PUBLIC gui/theme
        PUBLIC gui/tileset
        PUBLIC gui/layer
        PUBLIC gui/window
        PUBLIC gui/util
        PUBLIC gui/tools
        PUBLIC gui/settings
        PUBLIC gui/dialog
        PUBLIC gui/dialog/about
        PUBLIC gui/dialog/resize_map
        PUBLIC gui/dialog/save
        PUBLIC gui/dialog/open_map
        PUBLIC gui/dialog/open_tileset
        PUBLIC service
        PUBLIC service/save
        PUBLIC service/open
        PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(libTactile
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui
        PUBLIC Qt5::Xml)

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
            -Wconversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(libTactile PRIVATE
            /EHsc
            /MP
            /W3)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(libTactile PRIVATE
            -Xclang
            -Wall
            -Wextra
            -Wpedantic
            -Wdouble-promotion
            -Wswitch-default
            -Wswitch-enum
            -Wunused
            -Wuninitialized
            -Wconversion
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-unused-template
            -Wno-unused-macros
            -Wno-newline-eof
            -Wno-documentation-unknown-command)
endif ()

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(Tactile WIN32
            ${TACTILE_SOURCE_FILES}
            main.cpp)
else ()
    add_executable(Tactile
            ${TACTILE_SOURCE_FILES}
            main.cpp)
endif ()


add_dependencies(Tactile libTactile)

target_link_libraries(Tactile PUBLIC libTactile)

copy_directory_post_build(Tactile
        ${TACTILE_ROOT_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR})