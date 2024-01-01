// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/layers.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "tactile/foundation/misc/integer_conversion.hpp"
#include "tactile/testutil/ir/properties.hpp"

namespace tactile::testutil {

auto make_ir_tile_layer(const int id, const MatrixExtent extent) -> ir::Layer
{
  return {
    .meta = ir::Metadata {},
    .name = fmt::format("Tile Layer {}", id),
    .id = id,
    .type = LayerType::kTileLayer,
    .opacity = 1.0f,
    .row_count = extent.row_count,
    .col_count = extent.col_count,
    .tiles = make_tile_matrix(extent),
    .objects = Vector<ir::Object> {},
    .layers = Vector<ir::Layer> {},
    .visible = true,
  };
}

auto make_ir_object_layer(const int id) -> ir::Layer
{
  return {
    .meta = ir::Metadata {},
    .name = fmt::format("Object Layer {}", id),
    .id = id,
    .type = LayerType::kObjectLayer,
    .opacity = 1.0f,
    .row_count = 0,
    .col_count = 0,
    .tiles = TileMatrix {},
    .objects = Vector<ir::Object> {},
    .layers = Vector<ir::Layer> {},
    .visible = true,
  };
}

auto make_ir_group_layer(const int id, Vector<ir::Layer> layers) -> ir::Layer
{
  return {
    .meta = ir::Metadata {},
    .name = fmt::format("Group Layer {}", id),
    .id = id,
    .type = LayerType::kGroupLayer,
    .opacity = 1.0f,
    .row_count = 0,
    .col_count = 0,
    .tiles = TileMatrix {},
    .objects = Vector<ir::Object> {},
    .layers = std::move(layers),
    .visible = true,
  };
}

auto make_tile_matrix_with_increasing_tiles(const MatrixExtent extent,
                                            const int32 first_id) -> TileMatrix
{
  auto tile_matrix = make_tile_matrix(extent);

  const auto row_count = as_unsigned(extent.row_count);
  const auto col_count = as_unsigned(extent.col_count);

  int32 tile_id = first_id;
  for (usize row = 0; row < row_count; ++row) {
    for (usize col = 0; col < col_count; ++col) {
      tile_matrix[row][col].value = tile_id;
      ++tile_id;
    }
  }

  return tile_matrix;
}

auto make_complex_ir_group_layer(int id, const MatrixExtent extent) -> ir::Layer
{
  auto root = make_ir_group_layer(id++);
  root.meta.properties = make_basic_properties();

  auto g1 = make_ir_group_layer(id++);
  auto g2 = make_ir_group_layer(id++);
  auto g3 = make_ir_group_layer(id++);

  g3.layers.push_back(make_ir_tile_layer(id++, extent));
  g3.layers.push_back(make_ir_group_layer(id++));
  g3.layers.push_back(make_ir_object_layer(id++));

  g2.layers.push_back(std::move(g3));
  g2.layers.push_back(make_ir_object_layer(id++));

  g1.layers.push_back(make_ir_object_layer(id++));
  g1.layers.push_back(make_ir_tile_layer(id++, extent));
  g1.layers.push_back(std::move(g2));
  g1.layers.push_back(make_ir_tile_layer(id++, extent));

  root.layers.push_back(make_ir_tile_layer(id++, extent));
  root.layers.push_back(make_ir_object_layer(id));
  root.layers.push_back(std::move(g1));

  return root;
}

}  // namespace tactile::testutil
