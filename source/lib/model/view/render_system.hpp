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

#include "common/color.hpp"
#include "common/type/ecs.hpp"
#include "core/prelude.hpp"
#include "core/tiles/tile_pos.hpp"
#include "model/maps/map_components.hpp"
#include "model/registry.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "ui/render/canvas.hpp"

namespace tactile::sys {

void render_map(const Registry& registry, const ui::CanvasInfo& canvas, const Map& map);

void render_tileset(const Registry& registry,
                    const ui::CanvasInfo& canvas,
                    const Tileset& tileset);

void render_layer(const Registry& registry,
                  const ui::CanvasInfo& canvas,
                  const Map& map,
                  Entity parent_layer_entity,
                  Entity layer_entity);

void render_tile_layer(const Registry& registry,
                       const ui::CanvasInfo& canvas,
                       const Map& map,
                       Entity tile_layer_entity,
                       float opacity);

void render_tile(const Registry& registry,
                 const ui::CanvasInfo& canvas,
                 const Map& map,
                 TileID tile_id,
                 const TilePos& pos,
                 float opacity = 1.0f);

void render_object_layer(const Registry& registry,
                         const ui::CanvasInfo& canvas,
                         Entity object_layer_entity,
                         float opacity);

void render_object(const Registry& registry,
                   const ui::CanvasInfo& canvas,
                   Entity object_entity,
                   const Color& color);

}  // namespace tactile::sys
