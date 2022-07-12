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

#include "core/cmd/tileset/all.hpp"
#include "core/tileset/tileset_info.hpp"
#include "core/util/tiles.hpp"
#include "misc/panic.hpp"

namespace tactile {

TilesetDocument::TilesetDocument(const UUID& id, const TilesetInfo& info)
    : mTileset {std::make_shared<Tileset>(id, info)}
    , mContexts {mTileset->get_uuid()}
{
  mContexts.add_context(mTileset);
  mContexts.select(mTileset->get_uuid());
}

TilesetDocument::TilesetDocument(const TilesetInfo& info)
    : TilesetDocument {make_uuid(), info}
{}

auto TilesetDocument::get_contexts() -> ContextManager&
{
  return mContexts;
}

auto TilesetDocument::get_contexts() const -> const ContextManager&
{
  return mContexts;
}

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

void TilesetDocument::rename_tileset(std::string name)
{
  get_history().exec<RenameTilesetCmd>(mTileset, std::move(name));
}

}  // namespace tactile