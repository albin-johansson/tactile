#pragma once

namespace tactile {

/// \addtogroup commands
/// \{

struct command_id final
{
  enum Value {
    add_row,
    add_column,
    remove_row,
    remove_column,
    resize_map,

    add_layer,
    remove_layer,
    duplicate_layer,
    move_layer_up,
    move_layer_down,
    set_layer_opacity,
    set_layer_visible,
    rename_layer,

    add_property,
    remove_property,
    rename_property,
    update_property,
    change_property_type,

    stamp_sequence,
    eraser_sequence,
    bucket,

    add_tileset,
    remove_tileset,
    set_tileset_name,

    set_object_name,
    move_object,
    set_object_tag,
    set_object_visibility,

    create_component_def,
    remove_component_def,
    rename_component,

    create_component_def_attribute,
    remove_component_attribute,
    rename_component_attribute,
    set_component_attribute_type,
    update_component_attribute,
    duplicate_component_attribute,

    add_component,
    remove_component,
    update_component,
    reset_component
  };
};

/// \} End of group commands

}  // namespace tactile
