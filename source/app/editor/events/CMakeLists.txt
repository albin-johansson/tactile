cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-events CXX)

set(TACTILE_EDITOR_EVENT_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/command_events.hpp
    ${PROJECT_SOURCE_DIR}/component_events.hpp
    ${PROJECT_SOURCE_DIR}/edit_events.hpp
    ${PROJECT_SOURCE_DIR}/layer_events.hpp
    ${PROJECT_SOURCE_DIR}/map_events.hpp
    ${PROJECT_SOURCE_DIR}/object_events.hpp
    ${PROJECT_SOURCE_DIR}/property_events.hpp
    ${PROJECT_SOURCE_DIR}/quit_event.hpp
    ${PROJECT_SOURCE_DIR}/save_events.hpp
    ${PROJECT_SOURCE_DIR}/tileset_events.hpp
    ${PROJECT_SOURCE_DIR}/tool_events.hpp
    ${PROJECT_SOURCE_DIR}/view_events.hpp
    ${PROJECT_SOURCE_DIR}/viewport_events.hpp

    PARENT_SCOPE)