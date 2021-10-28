#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // entity

namespace Tactile {

struct Map final
{
  int32 row_count{};     ///< Total amount of rows.
  int32 column_count{};  ///< Total amount of columns.
  int tile_width{};      ///< Logical width of all tiles.
  int tile_height{};     ///< Logical height of all tiles.

  LayerID next_layer_id{1};    ///< The next available layer identifier.
  ObjectID next_object_id{1};  ///< The next available object identifier.

  int tile_layer_suffix{1};    ///< Incrementing tile layer suffix.
  int object_layer_suffix{1};  ///< Incrementing object layer suffix.
  int group_layer_suffix{1};   ///< Incrementing group layer suffix.
};

}  // namespace Tactile
