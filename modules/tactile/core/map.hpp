#pragma once

#include <entt/entt.hpp>  // entity
#include <tactile_def.hpp>

namespace Tactile {

struct Map final
{
  usize row_count{};     ///< Total amount of rows.
  usize column_count{};  ///< Total amount of columns.
  int32 tile_width{};    ///< Logical width of all tiles.
  int32 tile_height{};   ///< Logical height of all tiles.

  LayerID next_layer_id{1};    ///< The next available layer identifier.
  ObjectID next_object_id{1};  ///< The next available object identifier.

  int32 tile_layer_suffix{1};    ///< Incrementing tile layer suffix.
  int32 object_layer_suffix{1};  ///< Incrementing object layer suffix.
  int32 group_layer_suffix{1};   ///< Incrementing group layer suffix.
};

}  // namespace Tactile
