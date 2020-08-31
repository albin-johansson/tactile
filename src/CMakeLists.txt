cmake_minimum_required(VERSION 3.12)

project(tactile_src)

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
        misc/tactile_error.hpp
        misc/tactile_error.cpp
        misc/tactile_types.hpp
        misc/tactile_fwd.hpp
        prefs/setting.hpp

        model/core_fwd.hpp
        model/core_model.hpp
        model/core_model.cpp
        model/tile_layer_model.cpp
        model/tile_layer_model.hpp
        model/map_model.cpp
        model/map_model.hpp

        model/algo/flood_fill.hpp
        model/algo/flood_fill.cpp
        model/algo/algorithm.hpp

        model/tile/tile_size.hpp
        model/tile/tile_size.cpp

        model/tileset/tileset.cpp
        model/tileset/tileset.hpp
        model/tileset/tileset_model.cpp
        model/tileset/tileset_model.hpp

        model/tilemap/tilemap.hpp
        model/tilemap/tilemap.cpp
        model/tilemap/tile_layer.cpp
        model/tilemap/tile_layer.hpp
        model/tilemap/map_position.hpp

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

        gui/modal/about_dialog.cpp
        gui/modal/about_dialog.hpp
        gui/modal/settings_dialog.hpp
        gui/modal/settings_dialog.cpp
        gui/modal/resize_dialog.hpp
        gui/modal/resize_dialog.cpp
        gui/modal/about.ui
        gui/modal/resize_dialog.ui
        gui/modal/settings.ui

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
        gui/editor/editor_pane.ui

        command/command_stack.cpp
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

        prefs/theme.cpp
        prefs/theme.hpp

        nenya/nenya.hpp

        prefs/preferences.cpp
        prefs/preferences.hpp

        parse/parse_palette.cpp
        parse/parse_palette.hpp)

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

add_header_only_lib(libSemimap ${LIBRARY_DIR}/hogliux/semimap ${LIBRARY_DIR}/hogliux/semimap/semimap.h)

target_include_directories(libTactile
        PUBLIC ${TACTILE_ROOT_DIR}/src
        PUBLIC command
        PUBLIC app
        PUBLIC misc
        PUBLIC nenya
        PUBLIC model
        PUBLIC model/util
        PUBLIC model/tile
        PUBLIC model/tilemap
        PUBLIC model/tileset
        PUBLIC model/algo
        PUBLIC gui
        PUBLIC prefs
        PUBLIC parse
        PUBLIC gui/startup
        PUBLIC gui/toolbar
        PUBLIC gui/editor
        PUBLIC gui/theme
        PUBLIC gui/modal
        PUBLIC gui/tileset
        PUBLIC gui/window
        PUBLIC gui/util
        PUBLIC gui/tools
        #        PUBLIC ${Boost_INCLUDE_DIRS}
        )

target_link_libraries(libTactile
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Gui
        PUBLIC libGSL
        PUBLIC libSemimap)

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
            -fcxx-exceptions
            -fconcepts
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