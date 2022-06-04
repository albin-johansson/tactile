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
#include "core/components/attributes.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/systems/tilesets/tileset_document_system.hpp"
#include "core/systems/tilesets/tileset_system.hpp"

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
  auto& cache = ctx_get<comp::TilesetCache>(mRegistry);
  cache.source_to_render.clear();
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

}  // namespace tactile