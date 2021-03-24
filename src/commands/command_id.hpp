#pragma once

namespace tactile::cmd {

enum class command_id
{
  add_column = 1,
  add_layer,
  add_property,
  add_row,
  add_tileset,
  bucket_fill,
  change_property_type,
  eraser,
  remove_column,
  remove_layer,
  remove_property,
  remove_row,
  remove_tileset,
  rename_property,
  resize_map,
  stamp,
  update_property,
  duplicate_layer,
  change_opacity,
  move_layer_forward,
  move_layer_back,
  select_layer,
  set_layer_name,
  set_layer_visibility,
  set_tileset_name
};

}  // namespace tactile::cmd
