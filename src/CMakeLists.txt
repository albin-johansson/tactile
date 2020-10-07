cmake_minimum_required(VERSION 3.12)

project(tactile_src CXX)

set(TACTILE_SOURCE_FILES
        tactile.qrc

        fwd.hpp
        version.hpp

        app/app.cpp
        app/app.hpp
        app/app_connections.hpp
        app/app_connections.cpp
        app/set_style_sheet.hpp
        app/set_style_sheet.cpp
        app/setup_app.hpp
        app/setup_app.cpp

        core/types.hpp
        core/core_fwd.hpp
        core/model.hpp
        core/model.cpp
        core/tactile_error.hpp
        core/tactile_error.cpp
        core/iterate_tileset_selection.hpp
        core/position.hpp

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
        gui/tileset/tileset_dialog.cpp
        gui/tileset/tileset_dialog.hpp
        gui/tileset/tileset_dialog.ui
        gui/tileset/tileset_content_page.ui
        gui/tileset/tileset_empty_page.ui
        gui/tileset/open_tileset_image.cpp
        gui/tileset/open_tileset_image.hpp
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

        gui/about/about_dialog.cpp
        gui/about/about_dialog.hpp

        gui/settings/settings_dialog.hpp
        gui/settings/settings_dialog.cpp
        gui/settings/settings_dialog.ui

        gui/action/resize_map/resize_dialog.hpp
        gui/action/resize_map/resize_dialog.cpp
        gui/action/resize_map/resize_dialog.ui

        gui/action/save/save_as_dialog.cpp
        gui/action/save/save_as_dialog.hpp

        gui/action/open_map/open_map_dialog.cpp
        gui/action/open_map/open_map_dialog.hpp

        gui/about/about.ui

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
        command/abstract_command.cpp
        command/abstract_command.hpp
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
        command/row_col_command.cpp
        command/row_col_command.hpp

        prefs/setting.hpp
        prefs/theme.cpp
        prefs/theme.hpp
        prefs/preferences.cpp
        prefs/preferences.hpp
        parse/parse_palette.cpp
        parse/parse_palette.hpp

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
        util/to_string.hpp

        service/saves/save_tmx.cpp
        service/saves/save_tmx.hpp
        service/saves/save_service.cpp
        service/saves/save_service.hpp
        service/saves/save_json.cpp
        service/saves/save_json.hpp
        service/saves/export_options.hpp
        service/saves/tiled_version.hpp
        service/saves/open_map.cpp
        service/saves/open_map.hpp)

add_library(libTactile ${TACTILE_SOURCE_FILES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif ()

set(GSL_SOURCES
        ${LIBRARY_DIR}/microsoft/gsl/gsl
        ${LIBRARY_DIR}/microsoft/gsl/gsl_algorithm
        ${LIBRARY_DIR}/microsoft/gsl/gsl_assert
        ${LIBRARY_DIR}/microsoft/gsl/gsl_byte
        ${LIBRARY_DIR}/microsoft/gsl/gsl_util
        ${LIBRARY_DIR}/microsoft/gsl/multi_span
        ${LIBRARY_DIR}/microsoft/gsl/pointers
        ${LIBRARY_DIR}/microsoft/gsl/span
        ${LIBRARY_DIR}/microsoft/gsl/span_ext
        ${LIBRARY_DIR}/microsoft/gsl/string_span)
add_header_only_lib(libGSL ${LIBRARY_DIR}/microsoft ${GSL_SOURCES})

set(PUGIXML_SOURCES
        ${LIBRARY_DIR}/zeux/pugixml/pugixml.hpp
        ${LIBRARY_DIR}/zeux/pugixml/pugixml.cpp
        ${LIBRARY_DIR}/zeux/pugixml/pugiconfig.cpp)
add_header_only_lib(libPugiXML ${LIBRARY_DIR}/zeux ${PUGIXML_SOURCES})

target_include_directories(libTactile
        PUBLIC ${TACTILE_ROOT_DIR}/src
        PUBLIC app
        PUBLIC command
        PUBLIC prefs
        PUBLIC parse
        PUBLIC tool
        PUBLIC util
        PUBLIC core
        PUBLIC core/util
        PUBLIC core/tile
        PUBLIC core/map
        PUBLIC core/tileset
        PUBLIC core/algorithm
        PUBLIC gui
        PUBLIC gui/startup
        PUBLIC gui/toolbar
        PUBLIC gui/editor
        PUBLIC gui/theme
        PUBLIC gui/tileset
        PUBLIC gui/window
        PUBLIC gui/util
        PUBLIC gui/tools
        PUBLIC gui/about
        PUBLIC gui/settings
        PUBLIC gui/action
        PUBLIC gui/action/resize_map
        PUBLIC gui/action/save
        PUBLIC gui/action/open_map
        PUBLIC service
        PUBLIC service/saves
        PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(libTactile
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui
        PUBLIC Qt5::Xml
        PUBLIC libGSL
        PUBLIC libPugiXML)

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