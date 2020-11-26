#pragma once

namespace tactile::cmd {

enum class id
{
  add_row = 1,
  remove_row,
  add_column,
  remove_column,
  bucket_fill,
  eraser,
  stamp,
  resize_map,
  add_tileset,
  remove_tileset
};

}  // namespace tactile::cmd
