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

#include "tileset_document.hpp"

#include <utility>  // move

#include "core/common/ecs.hpp"
#include "core/components/animation.hpp"
#include "core/components/attributes.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/systems/animation_system.hpp"
#include "core/systems/tilesets/tileset_document_system.hpp"
#include "core/utils/tiles.hpp"

namespace tactile {

TilesetDocument::TilesetDocument(const UUID& id,
                                 const comp::Texture& texture,
                                 const Vector2i& tileSize)
    : ADocument{sys::new_tileset_document_registry(texture, tileSize)}
{
  ctx_get<comp::AttributeContext>(mRegistry).id = id;
}

void TilesetDocument::update()
{
  get_cache().display_tiles.clear();

  sys::update_animations(mRegistry);
}

auto TilesetDocument::tile_at(const TilePos& pos) const -> TileID
{
  const auto& ts = info();

  const auto row = pos.row();
  const auto col = pos.col();

  if (row >= 0 && col >= 0 && row < ts.row_count && col < ts.column_count) {
    return row * ts.column_count + col;
  }
  else {
    return empty_tile;
  }
}

auto TilesetDocument::tile_source(const TileIndex index) const -> const cen::irect&
{
  const auto& cache = get_cache();
  if (const auto iter = cache.source_rects.find(index);
      iter != cache.source_rects.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid tile index!"};
  }
}

auto TilesetDocument::get_displayed_tile(const TileIndex index) const -> TileIndex
{
  const auto& cache = get_cache();

  /* Check for already cached tile to render */
  if (const auto iter = cache.display_tiles.find(index);
      iter != cache.display_tiles.end()) {
    return iter->second;
  }

  if (const auto iter = cache.tiles.find(index); iter != cache.tiles.end()) {
    const auto entity = iter->second;

    if (const auto* animation = mRegistry.try_get<comp::Animation>(entity)) {
      const auto frameEntity = animation->current_frame();
      const auto& frame = checked_get<comp::AnimationFrame>(mRegistry, frameEntity);

      /* This cache is cleared before each frame */
      cache.display_tiles[index] = frame.tile_index;

      return frame.tile_index;
    }
  }

  return index;
}

auto TilesetDocument::info() const -> const comp::Tileset&
{
  return ctx_get<comp::Tileset>(mRegistry);
}

auto TilesetDocument::tile_size() const -> Vector2i
{
  const auto& tileset = info();
  return tileset.tile_size;
}

auto TilesetDocument::uv_size() const -> Vector2f
{
  const auto& uv = ctx_get<comp::UvTileSize>(mRegistry);
  return uv.size;
}

auto TilesetDocument::texture() const -> const comp::Texture&
{
  return ctx_get<comp::Texture>(mRegistry);
}

auto TilesetDocument::viewport() const -> const comp::Viewport&
{
  return ctx_get<comp::Viewport>(mRegistry);
}

auto TilesetDocument::get_cache() const -> const comp::TilesetCache&
{
  return ctx_get<comp::TilesetCache>(mRegistry);
}

}  // namespace tactile