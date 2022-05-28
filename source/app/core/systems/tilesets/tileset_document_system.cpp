/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "tileset_document_system.hpp"

#include "core/commands/command_stack.hpp"
#include "core/components/attributes.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/components/viewport.hpp"

namespace tactile::sys {

auto new_tileset_document_registry(const comp::Texture& texture, const Vector2i& tileSize)
    -> entt::registry
{
  entt::registry registry;

  auto& ctx = registry.ctx();
  ctx.emplace<CommandStack>();
  ctx.emplace<comp::Texture>(texture);
  ctx.emplace<comp::ActiveAttributeContext>();

  auto& tileset = ctx.emplace<comp::Tileset>();
  tileset.tile_width = tileSize.x;
  tileset.tile_height = tileSize.y;

  tileset.row_count = texture.height / tileset.tile_height;
  tileset.column_count = texture.width / tileset.tile_width;

  auto& cache = ctx.emplace<comp::TilesetCache>();
  // TODO do something with the cache?

  auto& viewport = ctx.emplace<comp::Viewport>();
  viewport.tile_width = static_cast<float>(tileSize.x);
  viewport.tile_height = static_cast<float>(tileSize.y);
  viewport.x_offset = 0;
  viewport.y_offset = 0;

  auto& uv = ctx.emplace<comp::UvTileSize>();
  uv.width = static_cast<float>(tileset.tile_width) / static_cast<float>(texture.width);
  uv.height =
      static_cast<float>(tileset.tile_height) / static_cast<float>(texture.height);

  auto& context = ctx.emplace<comp::AttributeContext>();
  context.id = make_uuid();
  context.name = "Tileset";

  return registry;
}

}  // namespace tactile::sys