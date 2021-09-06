#pragma once

#include <entt.hpp>  // entity

#include "aliases/col.hpp"
#include "aliases/layer_id.hpp"
#include "aliases/object_id.hpp"
#include "aliases/row.hpp"

namespace Tactile {

struct Map final
{
  row_t row_count;     ///< Total amount of rows.
  col_t column_count;  ///< Total amount of columns.
  int tile_width{};    ///< Logical width of all tiles.
  int tile_height{};   ///< Logical height of all tiles.

  LayerID next_layer_id{1};    ///< The next available layer identifier.
  ObjectID next_object_id{1};  ///< The next available object identifier.

  int tile_layer_suffix{1};    ///< Incrementing tile layer suffix.
  int object_layer_suffix{1};  ///< Incrementing object layer suffix.
};

}  // namespace Tactile
