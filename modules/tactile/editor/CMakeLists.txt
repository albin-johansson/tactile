cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor CXX)

add_subdirectory(commands)
add_subdirectory(events)
add_subdirectory(gui)
add_subdirectory(shortcuts)

set(TACTILE_EDITOR_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/document.hpp
    ${PROJECT_SOURCE_DIR}/model.cpp
    ${PROJECT_SOURCE_DIR}/model.hpp

    ${TACTILE_EDITOR_COMMAND_SOURCE_FILES}
    ${TACTILE_EDITOR_EVENT_SOURCE_FILES}
    ${TACTILE_EDITOR_GUI_SOURCE_FILES}
    ${TACTILE_EDITOR_SHORTCUT_SOURCE_FILES}

    PARENT_SCOPE)
