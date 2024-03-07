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

#include "model_impl.hpp"

#include <utility>  // move

#include "core/tile/tileset_bundle.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/cmd/tileset/all.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

DocumentModel::Impl::Impl(DocumentModel* parent)
    : mParent {parent}
{
}

void DocumentModel::Impl::each(const VisitorFunc& func) const
{
  mDocuments.each_open(func);
}

void DocumentModel::Impl::update()
{
  if (auto* doc = mDocuments.current_document()) {
    doc->update();
  }
}

auto DocumentModel::Impl::add_map(const Int2& tile_size, const TileExtent extent) -> UUID
{
  // TODO move this to a command

  TACTILE_ASSERT(tile_size.x() > 0);
  TACTILE_ASSERT(tile_size.y() > 0);

  auto map_document = std::make_shared<MapDocument>(tile_size, extent);
  map_document->set_component_index(std::make_shared<ComponentIndex>());

  const auto id = map_document->get_map().get_uuid();
  mDocuments.add_map_document(std::move(map_document));

  return id;
}

auto DocumentModel::Impl::add_tileset(const TilesetInfo& info) -> UUID
{
  if (auto* document = mDocuments.current_map_document()) {
    auto& map = document->get_map();
    const auto tileset_id = UUID::generate();

    auto& commands = document->get_history();
    commands.push<cmd::CreateTileset>(mParent, map.get_uuid(), tileset_id, info);

    return tileset_id;
  }
  else {
    throw Exception {"No active map to add tileset to!"};
  }
}

void DocumentModel::Impl::remove_tileset(const UUID& id)
{
  if (!mDocuments.is_tileset(id)) {
    throw Exception {"Document is not a tileset!"};
  }

  if (auto* document = mDocuments.current_map_document()) {
    document->get_history().push<cmd::RemoveTileset>(mParent, id);
  }
  else {
    throw Exception {"No active map!"};
  }
}

auto DocumentModel::Impl::restore_tileset(const TileID first_tile_id,
                                          const TilesetInfo& info) -> UUID
{
  if (auto* map_document = mDocuments.current_map_document()) {
    auto tileset_document = std::make_shared<TilesetDocument>(info);
    mDocuments.add_tileset_document(tileset_document);

    auto tileset = tileset_document->get_tileset_ptr();
    const auto tileset_id = tileset->get_uuid();

    auto& tilesets = map_document->get_map().get_tileset_bundle();
    tilesets.attach_tileset(tileset, first_tile_id, false);  // TODO embedded option

    map_document->get_contexts().add_context(std::move(tileset));

    return tileset_id;
  }
  else {
    throw Exception {"No active map to restore tileset to!"};
  }
}

void DocumentModel::Impl::set_command_capacity(const usize capacity)
{
  mDocuments.each([capacity](const Shared<Document>& document) {
    auto& commands = document->get_history();
    commands.set_capacity(capacity);
  });
}

auto DocumentModel::Impl::has_document(const UUID& id) const -> bool
{
  return mDocuments.is_document(id);
}

auto DocumentModel::Impl::has_document_with_path(const Path& path) const -> bool
{
  return mDocuments.has_document_with_path(path);
}

auto DocumentModel::Impl::get_id_for_path(const Path& path) const -> UUID
{
  const auto id = mDocuments.first_match([&](const Document& document) {
    return document.has_path() && document.get_path() == path;
  });

  if (id) {
    return *id;
  }
  else {
    throw Exception {"Found no document with the specified path!"};
  }
}

auto DocumentModel::Impl::require_active_document() const -> const Document&
{
  if (const auto* document = mDocuments.current_document()) {
    return *document;
  }
  else {
    throw Exception {"No document was active!"};
  }
}

auto DocumentModel::Impl::require_active_map() -> MapDocument&
{
  if (auto* document = mDocuments.current_map_document()) {
    return *document;
  }
  else {
    throw Exception {"No map document was active!"};
  }
}

auto DocumentModel::Impl::require_active_map() const -> const MapDocument&
{
  if (const auto* document = mDocuments.current_map_document()) {
    return *document;
  }
  else {
    throw Exception {"No map document was active!"};
  }
}

auto DocumentModel::Impl::require_active_tileset() -> TilesetDocument&
{
  if (auto* document = mDocuments.current_tileset_document()) {
    return *document;
  }
  else {
    throw Exception {"No tileset document was active!"};
  }
}

auto DocumentModel::Impl::require_active_tileset() const -> const TilesetDocument&
{
  if (const auto* document = mDocuments.current_tileset_document()) {
    return *document;
  }
  else {
    throw Exception {"No tileset document was active!"};
  }
}

auto DocumentModel::Impl::documents() -> DocumentManager&
{
  return mDocuments;
}

}  // namespace tactile
