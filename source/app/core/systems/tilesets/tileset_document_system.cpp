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
#include "core/components/contexts.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/components/viewport.hpp"
#include "core/utils/tiles.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _create_source_rect_cache(const comp::Tileset& info)
    -> HashMap<TileIndex, cen::irect>
{
  HashMap<TileIndex, cen::irect> cache;
  cache.reserve(static_cast<usize>(info.tile_count()));

  const auto n = info.tile_count();
  for (TileIndex index = 0; index < n; ++index) {
    const auto [row, col] = to_matrix_coords(index, info.column_count);

    const auto x = col * info.tile_size.x;
    const auto y = row * info.tile_size.y;

    cache[index] = cen::irect{x, y, info.tile_size.x, info.tile_size.y};
  }

  return cache;
}

}  // namespace

auto new_tileset_document_registry(const comp::Texture& texture, const Vector2i& tileSize)
    -> entt::registry
{
  entt::registry registry;

  auto& ctx = registry.ctx();
  ctx.emplace<CommandStack>();
  ctx.emplace<comp::Texture>(texture);
  ctx.emplace<comp::ActiveState>();
  ctx.emplace<comp::ContextMapping>();

  auto& tileset = ctx.emplace<comp::Tileset>();
  tileset.tile_size = tileSize;
  tileset.row_count = texture.size.y / tileset.tile_size.y;
  tileset.column_count = texture.size.x / tileset.tile_size.x;

  auto& cache = ctx.emplace<comp::TilesetCache>();
  cache.source_rects = _create_source_rect_cache(tileset);

  auto& viewport = ctx.emplace<comp::Viewport>();
  viewport.offset = {0, 0};
  viewport.tile_size = tileSize;

  auto& uv = ctx.emplace<comp::UvTileSize>();
  uv.size = Vector2f{tileset.tile_size} / Vector2f{texture.size};

  auto& context = ctx.emplace<comp::Context>();
  context.id = make_uuid();
  context.name = "Tileset";

  return registry;
}

}  // namespace tactile::sys