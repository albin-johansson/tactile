// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/ir.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/entity/registry.hpp"

namespace tactile::test {

void compare_meta(const Registry& registry, EntityID meta_id, const ir::Metadata& ir_meta);

void compare_object(const Registry& registry, EntityID object_id, const ir::Object& ir_object);

void compare_tile_layer(const Registry& registry,
                        EntityID layer_id,
                        const ir::Layer& ir_layer);

void compare_object_layer(const Registry& registry,
                          EntityID layer_id,
                          const ir::Layer& ir_layer);

void compare_group_layer(const Registry& registry,
                         EntityID layer_id,
                         const ir::Layer& ir_layer);

void compare_layer(const Registry& registry, EntityID layer_id, const ir::Layer& ir_layer);

void compare_tile(const Registry& registry, EntityID tile_id, const ir::Tile& ir_tile);

void compare_tileset(const Registry& registry,
                     EntityID tileset_id,
                     const ir::TilesetRef& ir_tileset_ref);

}  // namespace tactile::test
