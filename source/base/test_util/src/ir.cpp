// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/test_util/ir.hpp"

#include <format>   // format
#include <utility>  // move

#include "tactile/base/numeric/saturate_cast.hpp"

namespace tactile::test {

auto make_ir_tile_matrix(const MatrixExtent& extent) -> Vector<Vector<TileID>>
{
  const auto u_rows = saturate_cast<usize>(extent.rows);
  const auto u_cols = saturate_cast<usize>(extent.cols);

  Vector<TileID> tile_row {};
  tile_row.reserve(u_cols);
  tile_row.assign(u_cols, 0);

  Vector<Vector<TileID>> tile_matrix {};
  tile_matrix.reserve(u_rows);
  tile_matrix.assign(u_rows, tile_row);

  return tile_matrix;
}

auto make_ir_metadata() -> ir::Metadata
{
  return {
    .properties = {},
    .components = {},
  };
}

auto make_ir_object(const ObjectID id,
                    const ObjectType type,
                    const Float2& pos,
                    const Float2& size) -> ir::Object
{
  return {
    .meta = make_ir_metadata(),
    .name = std::format("Object {}", id),
    .id = id,
    .type = type,
    .position = pos,
    .size = size,
    .tag = "",
    .visible = true,
  };
}

auto make_ir_object_layer(const LayerID id, Vector<ir::Object> objects) -> ir::Layer
{
  return {
    .meta = make_ir_metadata(),
    .name = std::format("Object Layer {}", id),
    .id = id,
    .type = LayerType::kObjectLayer,
    .opacity = 1.0f,
    .extent = MatrixExtent {0, 0},
    .tiles = {},
    .objects = std::move(objects),
    .layers = {},
    .visible = true,
  };
}

auto make_ir_tile_layer(const LayerID id, const MatrixExtent& extent) -> ir::Layer
{
  return {
    .meta = make_ir_metadata(),
    .name = std::format("Tile Layer {}", id),
    .id = id,
    .type = LayerType::kTileLayer,
    .opacity = 1.0f,
    .extent = extent,
    .tiles = make_ir_tile_matrix(extent),
    .objects = {},
    .layers = {},
    .visible = true,
  };
}

auto make_ir_group_layer(const LayerID id, Vector<ir::Layer> layers) -> ir::Layer
{
  return {
    .meta = make_ir_metadata(),
    .name = std::format("Group Layer {}", id),
    .id = id,
    .type = LayerType::kGroupLayer,
    .opacity = 1.0f,
    .extent = MatrixExtent {0, 0},
    .tiles = {},
    .objects = {},
    .layers = std::move(layers),
    .visible = true,
  };
}

auto make_ir_tileset() -> ir::Tileset
{
  return {
    .meta = make_ir_metadata(),
    .name = "Dummy",
    .tile_size = {16, 16},
    .tile_count = 24,
    .column_count = 6,
    .image_size = Int2 {96, 64},
    .image_path = "assets/images/dummy.png",
    .tiles = {},
    .is_embedded = false,
  };
}

auto make_ir_tileset_ref(const TileID first_tile_id) -> ir::TilesetRef
{
  return {
    .tileset = make_ir_tileset(),
    .first_tile_id = first_tile_id,
  };
}

auto make_ir_tile_format() -> ir::TileFormat
{
  return {
    .encoding = TileEncoding::kPlainText,
    .compression = kNone,
    .compression_level = kNone,
  };
}

auto make_ir_map(const MatrixExtent& extent) -> ir::Map
{
  return {
    .meta = make_ir_metadata(),
    .name = "Map",
    .extent = extent,
    .tile_size = {50, 50},
    .next_layer_id = 1,
    .next_object_id = 1,
    .tile_format = make_ir_tile_format(),
    .components = {},
    .tilesets = {},
    .layers = {},
  };
}

}  // namespace tactile::test
