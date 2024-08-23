// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/id.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"

namespace tactile::test {

[[nodiscard]]
auto make_complex_ir_metadata(std::string name) -> ir::Metadata;

[[nodiscard]]
auto make_complex_ir_object(ObjectID id, ObjectType type) -> ir::Object;

[[nodiscard]]
auto make_complex_ir_object_layer(LayerID id, ObjectID& next_object_id) -> ir::Layer;

[[nodiscard]]
auto make_complex_ir_tile_layer(LayerID id, const MatrixExtent& extent = {4, 5}) -> ir::Layer;

[[nodiscard]]
auto make_complex_ir_group_layer(LayerID& next_layer_id,
                                 ObjectID& next_object_id,
                                 const MatrixExtent& extent = {4, 5}) -> ir::Layer;

[[nodiscard]]
auto make_complex_ir_tile(TileIndex index, ObjectID& next_object_id) -> ir::Tile;

[[nodiscard]]
auto make_complex_ir_tileset(TileID& next_tile_id, ObjectID& next_object_id) -> ir::TilesetRef;

[[nodiscard]]
auto make_complex_ir_map(const ir::TileFormat& tile_format) -> ir::Map;

}  // namespace tactile::test
