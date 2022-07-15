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

namespace tactile {

TilesetDocument::TilesetDocument(const UUID& id, const TilesetInfo& info)
    : mTileset {std::make_shared<Tileset>(id, info)}
    , mDelegate {mTileset->get_uuid()}
{
  auto& contexts = get_contexts();
  contexts.add_context(mTileset);
  contexts.select(mTileset->get_uuid());
}

TilesetDocument::TilesetDocument(const TilesetInfo& info)
    : TilesetDocument {make_uuid(), info}
{}

void TilesetDocument::update()
{
  // TODO ?
  //  get_cache().display_tiles.clear();
  //  sys::update_animations(mRegistry);
}

void TilesetDocument::rename_tileset(std::string name)
{
  get_history().exec<RenameTilesetCmd>(mTileset, std::move(name));
}

void TilesetDocument::set_component_index(Shared<ComponentIndex> index)
{
  mDelegate.set_component_index(std::move(index));
}

void TilesetDocument::set_name(std::string name)
{
  mTileset->set_name(std::move(name));
}

void TilesetDocument::set_path(fs::path path)
{
  mDelegate.set_path(std::move(path));
}

auto TilesetDocument::has_path() const -> bool
{
  return mDelegate.has_path();
}

auto TilesetDocument::get_component_index() -> Shared<ComponentIndex>
{
  return mDelegate.get_component_index();
}

auto TilesetDocument::view_component_index() const -> const ComponentIndex*
{
  return mDelegate.view_component_index();
}

auto TilesetDocument::get_contexts() -> ContextManager&
{
  return mDelegate.get_contexts();
}

auto TilesetDocument::get_contexts() const -> const ContextManager&
{
  return mDelegate.get_contexts();
}

auto TilesetDocument::get_history() -> CommandStack&
{
  return mDelegate.get_history();
}

auto TilesetDocument::get_history() const -> const CommandStack&
{
  return mDelegate.get_history();
}

auto TilesetDocument::get_viewport() -> Viewport&
{
  return mDelegate.get_viewport();
}

auto TilesetDocument::get_viewport() const -> const Viewport&
{
  return mDelegate.get_viewport();
}

auto TilesetDocument::get_name() const -> const std::string&
{
  return mTileset->get_name();
}

auto TilesetDocument::get_path() const -> const fs::path&
{
  return mDelegate.path();
}

auto TilesetDocument::get_type() const -> DocumentType
{
  return DocumentType::Tileset;
}

}  // namespace tactile