cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui CXX)

add_subdirectory(common)
add_subdirectory(components)
add_subdirectory(dialogs)
add_subdirectory(layers)
add_subdirectory(layout)
add_subdirectory(log)
add_subdirectory(menus)
add_subdirectory(properties)
add_subdirectory(rendering)
add_subdirectory(tilesets)
add_subdirectory(toolbar)
add_subdirectory(viewport)

set(TACTILE_EDITOR_GUI_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/alignment.cpp
    ${PROJECT_SOURCE_DIR}/alignment.hpp
    ${PROJECT_SOURCE_DIR}/dock_widget.cpp
    ${PROJECT_SOURCE_DIR}/dock_widget.hpp
    ${PROJECT_SOURCE_DIR}/icons.cpp
    ${PROJECT_SOURCE_DIR}/icons.hpp
    ${PROJECT_SOURCE_DIR}/scoped.hpp
    ${PROJECT_SOURCE_DIR}/textures.cpp
    ${PROJECT_SOURCE_DIR}/textures.hpp
    ${PROJECT_SOURCE_DIR}/themes.cpp
    ${PROJECT_SOURCE_DIR}/themes.hpp
    ${PROJECT_SOURCE_DIR}/viewport_area.cpp
    ${PROJECT_SOURCE_DIR}/viewport_area.hpp
    ${PROJECT_SOURCE_DIR}/widget_manager.cpp
    ${PROJECT_SOURCE_DIR}/widget_manager.hpp

    ${TACTILE_EDITOR_GUI_COMMON_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_COMPONENTS_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_DIALOGS_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_LAYERS_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_LAYOUT_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_LOG_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_MENUS_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_PROPERTIES_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_RENDERING_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_TILESETS_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_TOOLBAR_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_VIEWPORT_SOURCE_FILES}

    PARENT_SCOPE)