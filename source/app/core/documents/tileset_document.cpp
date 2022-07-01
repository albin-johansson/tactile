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

#include "core/commands/tilesets/all.hpp"
#include "core/tilesets/tileset_info.hpp"
#include "core/utils/tiles.hpp"
#include "misc/panic.hpp"

namespace tactile {

TilesetDocument::TilesetDocument(const UUID& id, const core::TilesetInfo& info)
    : mTileset{std::make_shared<core::Tileset>(id, info)}
{
  register_context(mTileset);
  select_context(mTileset->get_uuid());
}

TilesetDocument::TilesetDocument(const core::TilesetInfo& info)
    : TilesetDocument{make_uuid(), info}
{}

void TilesetDocument::register_context(Shared<core::IContext> context)
{
  const auto id = context->get_uuid();
  mContexts[id] = std::move(context);
}

void TilesetDocument::unregister_context(const UUID& id)
{
  if (const auto iter = mContexts.find(id); iter != mContexts.end()) {
    mContexts.erase(iter);

    if (mActiveContext == id) {
      mActiveContext = mTileset->get_uuid();
    }
  }
  else {
    throw TactileError{"Tried to unregister non-existent context!"};
  }
}

auto TilesetDocument::get_context(const UUID& id) -> Shared<core::IContext>
{
  return lookup_in(mContexts, id);
}

auto TilesetDocument::view_context(const UUID& id) const -> const core::IContext&
{
  return *lookup_in(mContexts, id);
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
  get_history().push<RenameTilesetCmd>(mTileset, std::move(name));
}

}  // namespace tactile