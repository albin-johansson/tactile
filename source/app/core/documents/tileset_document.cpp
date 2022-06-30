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

#include "core/tilesets/tileset_info.hpp"
#include "core/utils/tiles.hpp"

namespace tactile {

TilesetDocument::TilesetDocument(const UUID& id, const core::TilesetInfo& info)
    : mTileset{std::make_shared<core::Tileset>(id, info)}
{
  mContexts[mTileset->get_uuid()] = mTileset;
  select_context(mTileset->get_uuid());
}

TilesetDocument::TilesetDocument(const core::TilesetInfo& info)
    : TilesetDocument{make_uuid(), info}
{}

void TilesetDocument::update()
{
  //  get_cache().display_tiles.clear();
  //
  //  sys::update_animations(mRegistry);
}

void TilesetDocument::set_name(std::string name)
{
  mTileset->set_name(std::move(name));
}

auto TilesetDocument::get_name() const -> const std::string&
{
  return mTileset->get_name();
}

{

}

auto TilesetDocument::tile_source(const TileIndex index) const -> const cen::irect&
{
  throw TactileError{"Not implemented"};
  //  const auto& cache = get_cache();
  //  return lookup_in(cache.source_rects, index);
}

auto TilesetDocument::get_displayed_tile(const TileIndex index) const -> TileIndex
{
  /*const auto& cache = get_cache();

  */
  /* Check for already cached tile to render */ /*
 if (const auto iter = cache.display_tiles.find(index);
     iter != cache.display_tiles.end()) {
   return iter->second;
 }

 if (const auto iter = cache.tiles.find(index); iter != cache.tiles.end()) {
   const auto entity = iter->second;

   if (const auto* animation = mRegistry.try_get<comp::Animation>(entity)) {
     const auto frameEntity = animation->current_frame();
     const auto& frame = checked_get<comp::AnimationFrame>(mRegistry, frameEntity);

     */
  /* This cache is cleared before each frame */ /*
 cache.display_tiles[index] = frame.tile_index;

 return frame.tile_index;
}
}
*/
  return index;
}

}  // namespace tactile