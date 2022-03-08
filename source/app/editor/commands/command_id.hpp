/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace tactile {

/// \addtogroup commands
/// \{

struct CommandId final
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
    add_rectangle,
    add_ellipse,
    add_point,

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
