// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional
#include <vector>    // vector

#include "tactile/base/id.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/numeric/extent_2d.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::test {

[[nodiscard]]
auto make_ir_tile_matrix(const Extent2D& extent) -> std::vector<std::vector<TileID>>;

[[nodiscard]]
auto make_ir_metadata(std::string name) -> ir::Metadata;

[[nodiscard]]
auto make_ir_object(ObjectID id,
                    ObjectType type = ObjectType::kPoint,
                    const Float2& pos = {0, 0},
                    const Float2& size = {0, 0}) -> ir::Object;

[[nodiscard]]
auto make_ir_object_layer(LayerID id, std::vector<ir::Object> objects = {}) -> ir::Layer;

[[nodiscard]]
auto make_ir_tile_layer(LayerID id, const Extent2D& extent) -> ir::Layer;

[[nodiscard]]
auto make_ir_group_layer(LayerID id, std::vector<ir::Layer> layers = {}) -> ir::Layer;

[[nodiscard]]
auto make_ir_tile(TileIndex index) -> ir::Tile;

[[nodiscard]]
auto make_ir_tileset() -> ir::Tileset;

[[nodiscard]]
auto make_ir_tileset_ref(TileID& next_tile_id) -> ir::TilesetRef;

[[nodiscard]]
auto make_ir_tile_format() -> ir::TileFormat;

[[nodiscard]]
auto make_ir_map(const Extent2D& extent) -> ir::Map;

}  // namespace tactile::test
