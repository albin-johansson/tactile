cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-commands CXX)

set(TACTILE_EDITOR_COMMAND_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/command.cpp
    ${PROJECT_SOURCE_DIR}/command.hpp
    ${PROJECT_SOURCE_DIR}/command_id.hpp
    ${PROJECT_SOURCE_DIR}/command_stack.cpp
    ${PROJECT_SOURCE_DIR}/command_stack.hpp
    ${PROJECT_SOURCE_DIR}/commands.hpp

    ${PROJECT_SOURCE_DIR}/components/add_component_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/add_component_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/create_component_attribute_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/create_component_attribute_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/create_component_def_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/create_component_def_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/duplicate_component_attribute_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/duplicate_component_attribute_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/remove_component_attribute_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/remove_component_attribute_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/remove_component_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/remove_component_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/remove_component_def_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/remove_component_def_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/rename_component_attribute_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/rename_component_attribute_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/rename_component_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/rename_component_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/reset_component_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/reset_component_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/set_component_attribute_type_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/set_component_attribute_type_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/update_component_attribute_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/update_component_attribute_cmd.hpp
    ${PROJECT_SOURCE_DIR}/components/update_component_cmd.cpp
    ${PROJECT_SOURCE_DIR}/components/update_component_cmd.hpp

    ${PROJECT_SOURCE_DIR}/layers/add_layer_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/add_layer_cmd.hpp
    ${PROJECT_SOURCE_DIR}/layers/duplicate_layer_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/duplicate_layer_cmd.hpp
    ${PROJECT_SOURCE_DIR}/layers/move_layer_down_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/move_layer_down_cmd.hpp
    ${PROJECT_SOURCE_DIR}/layers/move_layer_up_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/move_layer_up_cmd.hpp
    ${PROJECT_SOURCE_DIR}/layers/remove_layer_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/remove_layer_cmd.hpp
    ${PROJECT_SOURCE_DIR}/layers/rename_layer_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/rename_layer_cmd.hpp
    ${PROJECT_SOURCE_DIR}/layers/set_layer_opacity_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/set_layer_opacity_cmd.hpp
    ${PROJECT_SOURCE_DIR}/layers/set_layer_visibility_cmd.cpp
    ${PROJECT_SOURCE_DIR}/layers/set_layer_visibility_cmd.hpp

    ${PROJECT_SOURCE_DIR}/maps/add_column_cmd.cpp
    ${PROJECT_SOURCE_DIR}/maps/add_column_cmd.hpp
    ${PROJECT_SOURCE_DIR}/maps/add_row_cmd.cpp
    ${PROJECT_SOURCE_DIR}/maps/add_row_cmd.hpp
    ${PROJECT_SOURCE_DIR}/maps/map_command_cache.cpp
    ${PROJECT_SOURCE_DIR}/maps/map_command_cache.hpp
    ${PROJECT_SOURCE_DIR}/maps/remove_column_cmd.cpp
    ${PROJECT_SOURCE_DIR}/maps/remove_column_cmd.hpp
    ${PROJECT_SOURCE_DIR}/maps/remove_row_cmd.cpp
    ${PROJECT_SOURCE_DIR}/maps/remove_row_cmd.hpp
    ${PROJECT_SOURCE_DIR}/maps/resize_map_cmd.cpp
    ${PROJECT_SOURCE_DIR}/maps/resize_map_cmd.hpp

    ${PROJECT_SOURCE_DIR}/objects/move_object_cmd.cpp
    ${PROJECT_SOURCE_DIR}/objects/move_object_cmd.hpp
    ${PROJECT_SOURCE_DIR}/objects/object_cmd.cpp
    ${PROJECT_SOURCE_DIR}/objects/object_cmd.hpp
    ${PROJECT_SOURCE_DIR}/objects/set_object_name_cmd.cpp
    ${PROJECT_SOURCE_DIR}/objects/set_object_name_cmd.hpp
    ${PROJECT_SOURCE_DIR}/objects/set_object_tag_cmd.cpp
    ${PROJECT_SOURCE_DIR}/objects/set_object_tag_cmd.hpp
    ${PROJECT_SOURCE_DIR}/objects/set_object_visibility_cmd.cpp
    ${PROJECT_SOURCE_DIR}/objects/set_object_visibility_cmd.hpp

    ${PROJECT_SOURCE_DIR}/properties/add_property_cmd.cpp
    ${PROJECT_SOURCE_DIR}/properties/add_property_cmd.hpp
    ${PROJECT_SOURCE_DIR}/properties/change_property_type_cmd.cpp
    ${PROJECT_SOURCE_DIR}/properties/change_property_type_cmd.hpp
    ${PROJECT_SOURCE_DIR}/properties/remove_property_cmd.cpp
    ${PROJECT_SOURCE_DIR}/properties/remove_property_cmd.hpp
    ${PROJECT_SOURCE_DIR}/properties/rename_property_cmd.cpp
    ${PROJECT_SOURCE_DIR}/properties/rename_property_cmd.hpp
    ${PROJECT_SOURCE_DIR}/properties/update_property_cmd.cpp
    ${PROJECT_SOURCE_DIR}/properties/update_property_cmd.hpp

    ${PROJECT_SOURCE_DIR}/tilesets/add_tileset_cmd.cpp
    ${PROJECT_SOURCE_DIR}/tilesets/add_tileset_cmd.hpp
    ${PROJECT_SOURCE_DIR}/tilesets/remove_tileset_cmd.cpp
    ${PROJECT_SOURCE_DIR}/tilesets/remove_tileset_cmd.hpp
    ${PROJECT_SOURCE_DIR}/tilesets/set_tileset_name_cmd.cpp
    ${PROJECT_SOURCE_DIR}/tilesets/set_tileset_name_cmd.hpp

    ${PROJECT_SOURCE_DIR}/tools/bucket_cmd.cpp
    ${PROJECT_SOURCE_DIR}/tools/bucket_cmd.hpp
    ${PROJECT_SOURCE_DIR}/tools/eraser_sequence_cmd.cpp
    ${PROJECT_SOURCE_DIR}/tools/eraser_sequence_cmd.hpp
    ${PROJECT_SOURCE_DIR}/tools/stamp_sequence_cmd.cpp
    ${PROJECT_SOURCE_DIR}/tools/stamp_sequence_cmd.hpp

    PARENT_SCOPE)
