/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "core/ecs/entity.hpp"
#include "core/ecs/registry.hpp"
#include "io/ir/map/map_ir.hpp"
#include "model/components/component_components.hpp"
#include "model/contexts/context_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "tactile/core/common/prelude.hpp"

namespace tactile {

void convert_context_to_ir(const Registry& registry,
                           Entity component_set_entity,
                           const Context& context,
                           ContextIR& ir_context);

void convert_object_to_ir(const Registry& registry,
                          Entity component_set_entity,
                          Entity object_entity,
                          ObjectIR& ir_object);

void convert_tile_layer_to_ir(const Registry& registry,
                              Entity layer_entity,
                              TileLayerIR& ir_tile_layer);

void convert_object_layer_to_ir(const Registry& registry,
                                Entity component_set_entity,
                                Entity layer_entity,
                                ObjectLayerIR& ir_object_layer);

void convert_group_layer_to_ir(const Registry& registry,
                               Entity component_set_entity,
                               Entity layer_entity,
                               GroupLayerIR& ir_group);

void convert_layer_to_ir(const Registry& registry,
                         Entity component_set_entity,
                         Entity layer_entity,
                         usize index,
                         LayerIR& ir_layer);

void convert_layers_to_ir(const Registry& registry,
                          Entity component_set_entity,
                          const Map& map,
                          MapIR& ir_map);

void convert_tile_animation_to_ir(const TileAnimation& animation, TileIR& ir_tile);

void convert_tiles_to_ir(const Registry& registry,
                         Entity component_set_entity,
                         const Tileset& tileset,
                         TilesetIR& ir_tileset);

void convert_tileset_to_ir(const Registry& registry,
                           Entity component_set_entity,
                           Entity attached_tileset_entity,
                           TilesetIR& ir_tileset);

void convert_tilesets_to_ir(const Registry& registry,
                            Entity component_set_entity,
                            const Map& map,
                            MapIR& ir_map);

void convert_components_to_ir(const Registry& registry,
                              const ComponentSet& component_set,
                              ComponentMap& ir_components);

void convert_tile_format_to_ir(const TileFormat& format, TileFormatIR& ir_format);

[[nodiscard]] auto convert_map_document_to_ir(const Registry& registry,
                                              Entity map_document_entity) -> MapIR;

}  // namespace tactile