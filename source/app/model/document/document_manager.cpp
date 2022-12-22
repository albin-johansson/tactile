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

#include "document_manager.hpp"

#include <algorithm>  // any_of
#include <utility>    // move

#include "core/tile/tileset_bundle.hpp"
#include "core/util/assoc.hpp"
#include "debug/assert.hpp"
#include "debug/panic.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile {

void DocumentManager::each(const DocVisitorFn& op) const
{
  for (const auto& [id, doc]: mDocuments) {
    op(doc);
  }
}

void DocumentManager::each_open(const IdVisitorFn& op) const
{
  for (const auto& id: mOpenDocuments) {
    op(id);
  }
}

void DocumentManager::add_map(Shared<MapDocument> document)
{
  if (!document) {
    throw TactileError {"Cannot store null map document!"};
  }

  const auto id = document->get_map().get_uuid();

  mDocuments[id] = document;
  mMaps[id] = std::move(document);

  mOpenDocuments.insert(id);
  mActiveDocument = id;
}

void DocumentManager::add_tileset(Shared<TilesetDocument> document)
{
  if (!document) {
    throw TactileError {"Cannot store null tileset document!"};
  }

  const auto id = document->get_tileset()->get_uuid();

  mDocuments[id] = document;
  mTilesets[id] = std::move(document);
}

auto DocumentManager::remove_map(const UUID& id) -> Shared<MapDocument>
{
  if (auto iter = mMaps.find(id); iter != mMaps.end()) {
    auto document = iter->second;

    mMaps.erase(iter);
    mDocuments.erase(id);
    mOpenDocuments.erase(id);

    if (mActiveDocument == id) {
      select_another_document();
    }

    remove_unused_tilesets_from(document->get_map().tileset_bundle());
    TACTILE_ASSERT(!mActiveDocument || is_document(*mActiveDocument));

    return document;
  }

  return nullptr;
}

auto DocumentManager::remove_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  if (auto iter = mTilesets.find(id); iter != mTilesets.end()) {
    auto document = iter->second;

    mTilesets.erase(iter);
    mDocuments.erase(id);
    mOpenDocuments.erase(id);

    if (mActiveDocument == id) {
      select_another_document();
    }

    return document;
  }

  return nullptr;
}

void DocumentManager::select_another_document()
{
  mActiveDocument.reset();

  if (!mOpenDocuments.empty()) {
    mActiveDocument = *mOpenDocuments.begin();
  }
}

void DocumentManager::select(const UUID& id)
{
  if (is_document(id)) [[likely]] {
    mActiveDocument = id;
  }
  else {
    throw TactileError {"Tried to select invalid document!"};
  }
}

void DocumentManager::open(const UUID& id)
{
  if (!has_key(mDocuments, id)) [[unlikely]] {
    throw TactileError {"Tried to open invalid document!"};
  }

  mOpenDocuments.insert(id);

  if (!mActiveDocument) {
    mActiveDocument = id;
  }
}

void DocumentManager::close(const UUID& id)
{
  mOpenDocuments.erase(id);

  // Maps are removed when closed, unlike tilesets
  if (has_key(mMaps, id)) {
    remove_map(id);
  }

  if (mActiveDocument == id) {
    select_another_document();
  }
}

void DocumentManager::remove_unused_tilesets_from(const TilesetBundle& bundle)
{
  for (const auto& [id, tileset]: bundle) {
    if (!is_tileset_used(id)) {
      remove_tileset(id);
    }
  }
}

auto DocumentManager::current() const -> const Maybe<UUID>&
{
  return mActiveDocument;
}

auto DocumentManager::current_document() -> Document*
{
  return mActiveDocument ? lookup_in(mDocuments, *mActiveDocument).get() : nullptr;
}

auto DocumentManager::current_document() const -> const Document*
{
  return mActiveDocument ? lookup_in(mDocuments, *mActiveDocument).get() : nullptr;
}

auto DocumentManager::current_map() -> MapDocument*
{
  if (mActiveDocument) {
    if (auto iter = mMaps.find(*mActiveDocument); iter != mMaps.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentManager::current_map() const -> const MapDocument*
{
  if (mActiveDocument) {
    if (auto iter = mMaps.find(*mActiveDocument); iter != mMaps.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentManager::current_tileset() -> TilesetDocument*
{
  if (mActiveDocument) {
    if (auto iter = mTilesets.find(*mActiveDocument); iter != mTilesets.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentManager::current_tileset() const -> const TilesetDocument*
{
  if (mActiveDocument) {
    if (auto iter = mTilesets.find(*mActiveDocument); iter != mTilesets.end()) {
      return iter->second.get();
    }
  }

  return nullptr;
}

auto DocumentManager::is_open(const UUID& id) const -> bool
{
  return mOpenDocuments.find(id) != mOpenDocuments.end();
}

auto DocumentManager::is_map_active() const -> bool
{
  return mActiveDocument && is_map(*mActiveDocument);
}

auto DocumentManager::is_tileset_active() const -> bool
{
  return mActiveDocument && is_tileset(*mActiveDocument);
}

auto DocumentManager::is_document(const UUID& id) const -> bool
{
  return has_key(mDocuments, id);
}

auto DocumentManager::is_map(const UUID& id) const -> bool
{
  return has_key(mMaps, id);
}

auto DocumentManager::is_tileset(const UUID& id) const -> bool
{
  return has_key(mTilesets, id);
}

auto DocumentManager::is_tileset_used(const UUID& id) const -> bool
{
  return std::any_of(mMaps.begin(), mMaps.end(), [&](const auto& pair) {
    const auto& map = pair.second->get_map();
    return map.tileset_bundle().has_tileset(id);
  });
}

auto DocumentManager::has_with_path(const Path& path) const -> bool
{
  return std::any_of(mDocuments.begin(), mDocuments.end(), [&](const auto& pair) {
    const auto& document = pair.second;
    return document->has_path() && document->get_path() == path;
  });
}

auto DocumentManager::get_document(const UUID& id) -> Shared<Document>
{
  return lookup_in(mDocuments, id);
}

auto DocumentManager::get_map(const UUID& id) -> Shared<MapDocument>
{
  return lookup_in(mMaps, id);
}

auto DocumentManager::get_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  return lookup_in(mTilesets, id);
}

auto DocumentManager::view_document(const UUID& id) const -> const Document&
{
  return *lookup_in(mDocuments, id);
}

auto DocumentManager::view_map(const UUID& id) const -> const MapDocument&
{
  return *lookup_in(mMaps, id);
}

auto DocumentManager::view_tileset(const UUID& id) const -> const TilesetDocument&
{
  return *lookup_in(mTilesets, id);
}

auto DocumentManager::first_match(const PredicateFn& pred) const -> Maybe<UUID>
{
  for (const auto& [id, document]: mDocuments) {
    if (pred(*document)) {
      return id;
    }
  }

  return nothing;
}

}  // namespace tactile