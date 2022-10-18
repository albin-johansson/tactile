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

#include "model_impl.hpp"

#include <algorithm>  // find, any_of
#include <utility>    // move

#include <spdlog/spdlog.h>

#include "core/util/assoc.hpp"
#include "core/util/fmt.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/cmd/tileset/all.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile {

DocumentModel::Impl::Impl(DocumentModel* parent)
    : mParent {parent}
{
}

void DocumentModel::Impl::update()
{
  if (mActiveDocument) {
    lookup_in(mDocuments, *mActiveDocument)->update();
  }
}

void DocumentModel::Impl::each(const DocumentModel::VisitorFunc& func) const
{
  for (const auto& id : mOpenDocuments) {
    func(id);
  }
}

auto DocumentModel::Impl::add_map(const Int2& tile_size,
                                  const usize rows,
                                  const usize columns) -> UUID
{
  // TODO move this to a command

  TACTILE_ASSERT(tile_size.x > 0);
  TACTILE_ASSERT(tile_size.y > 0);

  auto map_document = std::make_shared<MapDocument>(tile_size, rows, columns);
  map_document->set_component_index(std::make_shared<ComponentIndex>());

  auto& map = map_document->get_map();

  register_map(map_document);
  mOpenDocuments.push_back(map.uuid());

  return map.uuid();
}

auto DocumentModel::Impl::add_tileset(const TilesetInfo& info) -> UUID
{
  if (mActiveDocument) {
    auto map_document = get_map(*mActiveDocument);
    auto& map = map_document->get_map();

    const auto tileset_id = make_uuid();

    auto& commands = map_document->get_history();
    commands.push<cmd::CreateTileset>(mParent, map.uuid(), tileset_id, info);

    return tileset_id;
  }
  else {
    throw TactileError {"No active map to add tileset to!"};
  }
}

void DocumentModel::Impl::remove_tileset(const UUID& id)
{
  if (!is_tileset(id)) {
    throw TactileError {"Document is not a tileset!"};
  }

  if (mActiveDocument && is_map(*mActiveDocument)) {
    auto map = get_map(*mActiveDocument);
    map->get_history().push<cmd::RemoveTileset>(mParent, id);
  }
  else {
    throw TactileError {"No active map!"};
  }
}

auto DocumentModel::Impl::restore_tileset(TileID first_tile_id, const TilesetInfo& info)
    -> UUID
{
  if (mActiveDocument) {
    auto map_document = get_map(*mActiveDocument);
    auto& map = map_document->get_map();

    auto tileset_document = std::make_shared<TilesetDocument>(info);
    register_tileset(tileset_document);

    auto tileset = tileset_document->get_tileset();
    const auto tileset_id = tileset->uuid();

    auto& tilesets = map.tileset_bundle();
    tilesets.attach_tileset(tileset, first_tile_id, false);  // TODO embedded option
    map_document->get_contexts().add_context(std::move(tileset));

    return tileset_id;
  }
  else {
    throw TactileError {"No active map to restore tileset to!"};
  }
}

void DocumentModel::Impl::select_document(const UUID& id)
{
  if (has_document(id)) [[likely]] {
    mActiveDocument = id;
  }
  else {
    throw TactileError {"Invalid document identifier!"};
  }
}

void DocumentModel::Impl::open_document(const UUID& id)
{
  if (!has_document(id)) {
    throw TactileError {"Cannot open document that does not exist!"};
  }

  if (!is_open(id)) {
    mOpenDocuments.push_back(id);

    if (!mActiveDocument) {
      mActiveDocument = id;
    }
  }
  else {
    throw TactileError {"Cannot open document that was already open!"};
  }
}

void DocumentModel::Impl::close_document(const UUID& id)
{
  if (!has_document(id)) {
    throw TactileError {"Cannot close document that does not exist!"};
  }

  const auto* iter = std::find(mOpenDocuments.begin(), mOpenDocuments.end(), id);
  if (iter != mOpenDocuments.end()) {
    mOpenDocuments.erase(iter);

    if (mActiveDocument == id) {
      select_another_document();
    }

    // Only map documents are removed when they are closed
    if (is_map(id)) {
      unregister_map(id);
    }

    TACTILE_ASSERT(!is_open(id));
  }
  else {
    throw TactileError {"Cannot close document that was not open!"};
  }
}

void DocumentModel::Impl::set_command_capacity(const usize capacity)
{
  for (auto& [id, document] : mDocuments) {
    auto& commands = document->get_history();
    commands.set_capacity(capacity);
  }
}

auto DocumentModel::Impl::has_document(const UUID& id) const -> bool
{
  return has_key(mDocuments, id);
}

auto DocumentModel::Impl::has_document_with_path(const Path& path) const -> bool
{
  return std::any_of(mDocuments.begin(), mDocuments.end(), [&](const auto& pair) {
    const auto& document = pair.second;
    return document->has_path() && document->get_path() == path;
  });
}

auto DocumentModel::Impl::get_id_for_path(const Path& path) const -> UUID
{
  for (const auto& [id, document] : mDocuments) {
    if (document->has_path() && document->get_path() == path) {
      return id;
    }
  }

  throw TactileError {"No document with the specified path!"};
}

auto DocumentModel::Impl::has_active_document() const -> bool
{
  return mActiveDocument.has_value();
}

auto DocumentModel::Impl::is_map_active() const -> bool
{
  return mActiveDocument && is_map(*mActiveDocument);
}

auto DocumentModel::Impl::is_tileset_active() const -> bool
{
  return mActiveDocument && is_tileset(*mActiveDocument);
}

auto DocumentModel::Impl::active_document() -> Document*
{
  if (mActiveDocument) {
    TACTILE_ASSERT(has_document(*mActiveDocument));
    return mDocuments.at(*mActiveDocument).get();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::Impl::active_document() const -> const Document*
{
  if (mActiveDocument) {
    TACTILE_ASSERT(has_document(*mActiveDocument));
    return mDocuments.at(*mActiveDocument).get();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::Impl::active_map() -> MapDocument*
{
  if (mActiveDocument) {
    const auto iter = find_in(mMaps, *mActiveDocument);
    if (iter != mMaps.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentModel::Impl::active_map() const -> const MapDocument*
{
  if (mActiveDocument) {
    const auto iter = find_in(mMaps, *mActiveDocument);
    if (iter != mMaps.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentModel::Impl::active_tileset() -> TilesetDocument*
{
  if (mActiveDocument) {
    const auto iter = find_in(mTilesets, *mActiveDocument);
    if (iter != mTilesets.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentModel::Impl::active_tileset() const -> const TilesetDocument*
{
  if (mActiveDocument) {
    const auto iter = find_in(mTilesets, *mActiveDocument);
    if (iter != mTilesets.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentModel::Impl::require_active_document() const -> const Document&
{
  if (const auto* document = active_document()) {
    return *document;
  }
  else {
    throw TactileError {"No document was active!"};
  }
}

auto DocumentModel::Impl::require_active_map() -> MapDocument&
{
  if (auto* document = active_map()) {
    return *document;
  }
  else {
    throw TactileError {"No map document was active!"};
  }
}

auto DocumentModel::Impl::require_active_map() const -> const MapDocument&
{
  if (const auto* document = active_map()) {
    return *document;
  }
  else {
    throw TactileError {"No map document was active!"};
  }
}

auto DocumentModel::Impl::require_active_tileset() -> TilesetDocument&
{
  if (auto* document = active_tileset()) {
    return *document;
  }
  else {
    throw TactileError {"No tileset document was active!"};
  }
}

auto DocumentModel::Impl::require_active_tileset() const -> const TilesetDocument&
{
  if (const auto* document = active_tileset()) {
    return *document;
  }
  else {
    throw TactileError {"No tileset document was active!"};
  }
}

auto DocumentModel::Impl::is_open(const UUID& id) const -> bool
{
  const auto* iter = std::find(mOpenDocuments.begin(), mOpenDocuments.end(), id);
  return iter != mOpenDocuments.end();
}

auto DocumentModel::Impl::is_map(const UUID& id) const -> bool
{
  return has_key(mMaps, id);
}

auto DocumentModel::Impl::is_tileset(const UUID& id) const -> bool
{
  return has_key(mTilesets, id);
}

auto DocumentModel::Impl::get_document(const UUID& id) -> Shared<Document>
{
  return lookup_in(mDocuments, id);
}

auto DocumentModel::Impl::get_map(const UUID& id) -> Shared<MapDocument>
{
  return lookup_in(mMaps, id);
}

auto DocumentModel::Impl::get_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  return lookup_in(mTilesets, id);
}

auto DocumentModel::Impl::view_document(const UUID& id) const -> const Document&
{
  return *lookup_in(mDocuments, id);
}

auto DocumentModel::Impl::view_map(const UUID& id) const -> const MapDocument&
{
  return *lookup_in(mMaps, id);
}

auto DocumentModel::Impl::view_tileset(const UUID& id) const -> const TilesetDocument&
{
  return *lookup_in(mTilesets, id);
}

auto DocumentModel::Impl::active_document_id() const -> Maybe<UUID>
{
  return mActiveDocument;
}

void DocumentModel::Impl::select_another_document()
{
  mActiveDocument.reset();
  if (!mOpenDocuments.empty()) {
    mActiveDocument = mOpenDocuments.front();
  }
}

void DocumentModel::Impl::remove_unreferenced_tilesets(const MapDocument& doc)
{
  const auto& tilesets = doc.get_map().tileset_bundle();
  for (const auto& [ts_id, ts_doc] : tilesets) {
    if (!is_tileset_referenced(ts_id)) {
      unregister_tileset(ts_id);

      if (mActiveDocument == ts_id) {
        select_another_document();
      }
    }
  }
}

void DocumentModel::Impl::register_map(Shared<MapDocument> document)
{
  const auto id = document->get_map().uuid();
  mDocuments[id] = document;
  mMaps[id] = std::move(document);
}

void DocumentModel::Impl::register_tileset(Shared<TilesetDocument> document)
{
  const auto id = document->view_tileset().uuid();
  mDocuments[id] = document;
  mTilesets[id] = std::move(document);
}

auto DocumentModel::Impl::unregister_map(const UUID& id) -> Shared<MapDocument>
{
  spdlog::debug("Removing map document {}", id);
  auto document = get_map(id);

  if (mActiveDocument == id) {
    mActiveDocument.reset();
  }

  mDocuments.erase(id);
  mMaps.erase(id);
  erase(mOpenDocuments, id);

  remove_unreferenced_tilesets(*document);
  TACTILE_ASSERT(!mActiveDocument || has_document(*mActiveDocument));

  return document;
}

auto DocumentModel::Impl::unregister_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  auto tileset = get_tileset(id);

  mDocuments.erase(id);
  mTilesets.erase(id);
  erase(mOpenDocuments, id);

  return tileset;
}

auto DocumentModel::Impl::is_tileset_referenced(const UUID& tileset_id) const -> bool
{
  return std::any_of(mMaps.begin(), mMaps.end(), [&](const auto& pair) {
    const auto& map = pair.second->get_map();
    return map.tileset_bundle().has_tileset(tileset_id);
  });
}

}  // namespace tactile