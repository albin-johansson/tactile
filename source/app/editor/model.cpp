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

#include "model.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "core/components/map_info.hpp"
#include "core/systems/animation_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/tools/tool_manager.hpp"
#include "editor/commands/command_stack.hpp"
#include "editor/commands/commands.hpp"
#include "editor/documents/map_document.hpp"
#include "editor/documents/tileset_document.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

void DocumentModel::update()
{
  if (auto* document = active_document()) {
    document->update();
  }
}

void DocumentModel::each(const VisitorFunc& func) const
{
  // TODO handle open documents?
  for (auto& [id, document] : mDocuments) {
    func(id);
  }
}

auto DocumentModel::add_map(const Vector2i& tileSize,
                            const usize rows,
                            const usize columns) -> UUID
{
  // TODO move this to a command

  TACTILE_ASSERT(tileSize.x > 0);
  TACTILE_ASSERT(tileSize.y > 0);

  auto map = std::make_shared<MapDocument>(tileSize, rows, columns);
  register_map(map);

  return map->id();
}

auto DocumentModel::add_tileset(const comp::Texture& texture, const Vector2i& tileSize)
    -> UUID
{
  if (mActiveDocument) {
    auto map = get_map(*mActiveDocument);
    const auto tilesetId = make_uuid();

    auto& commands = map->get_history();
    commands.push<AddTilesetCmd>(this, map->id(), tilesetId, texture, tileSize);

    return tilesetId;
  }
  else {
    throw TactileError{"No active map to add tileset to!"};
  }
}

void DocumentModel::attach_tileset_to(const UUID& mapId, const UUID& tilesetId)
{
  auto map = get_map(mapId);
  auto tileset = get_tileset(tilesetId);
  sys::attach_tileset(map->get_registry(), tilesetId, tileset->info());
}

void DocumentModel::detach_tileset_from(const UUID& mapId, const UUID& tilesetId)
{
  // TODO
}

void DocumentModel::select_document(const UUID& id)
{
  if (mDocuments.contains(id)) {
    mActiveDocument = id;
  }
  else {
    throw TactileError{"Invalid document identifier!"};
  }
}

void DocumentModel::remove_document(const UUID& id)
{
  TACTILE_ASSERT(mDocuments.contains(id));

  if (mActiveDocument == id) {
    mActiveDocument.reset();
  }

  if (is_map(id)) {
    auto map = get_map(id);
    auto& registry = map->get_registry();
    for (auto&& [entity, ref] : registry.view<comp::TilesetRef>().each()) {
      // TODO close if embedded
    }
  }

  mDocuments.erase(id);
  mMaps.erase(id);
  mTilesets.erase(id);

  if (!mActiveDocument && !mDocuments.empty()) {
    mActiveDocument = mDocuments.begin()->first;
  }
}

auto DocumentModel::has_document_with_path(const std::filesystem::path& path) const
    -> bool
{
  for (auto&& [id, document] : mDocuments) {
    if (document->get_path() == path) {
      return true;
    }
  }

  return false;
}

auto DocumentModel::get_document(const UUID& id) -> Shared<ADocument>
{
  if (const auto iter = mDocuments.find(id); iter != mDocuments.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid document identifier!"};
  }
}

auto DocumentModel::get_document(const UUID& id) const -> Shared<const ADocument>
{
  if (const auto iter = mDocuments.find(id); iter != mDocuments.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid document identifier!"};
  }
}

auto DocumentModel::active_document_id() const -> Maybe<UUID>
{
  return mActiveDocument;
}

auto DocumentModel::has_active_document() const -> bool
{
  return mActiveDocument.has_value();
}

auto DocumentModel::is_map_active() const -> bool
{
  if (mActiveDocument) {
    return is_map(*mActiveDocument);
  }
  else {
    return false;
  }
}

auto DocumentModel::is_tileset_active() const -> bool
{
  if (mActiveDocument) {
    return is_tileset(*mActiveDocument);
  }
  else {
    return false;
  }
}

auto DocumentModel::active_document() -> ADocument*
{
  if (mActiveDocument) {
    TACTILE_ASSERT(mDocuments.contains(*mActiveDocument));
    return mDocuments.at(*mActiveDocument).get();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::active_document() const -> const ADocument*
{
  if (mActiveDocument) {
    TACTILE_ASSERT(mDocuments.contains(*mActiveDocument));
    return mDocuments.at(*mActiveDocument).get();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::get_active_document() const -> const ADocument&
{
  if (const auto* document = active_document()) {
    return *document;
  }
  else {
    throw TactileError{"No active document!"};
  }
}

auto DocumentModel::active_registry() -> entt::registry*
{
  if (auto* document = active_document()) {
    return &document->get_registry();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::active_registry() const -> const entt::registry*
{
  if (const auto* document = active_document()) {
    return &document->get_registry();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::get_registry(const UUID& documentId) const -> const entt::registry&
{
  if (const auto iter = mDocuments.find(documentId); iter != mDocuments.end()) {
    return iter->second->get_registry();
  }
  else {
    throw TactileError{"Invalid document identifier!"};
  }
}

auto DocumentModel::get_active_registry() -> entt::registry&
{
  if (auto* registry = active_registry()) {
    return *registry;
  }
  else {
    throw TactileError{"No active registry!"};
  }
}

auto DocumentModel::get_active_registry() const -> const entt::registry&
{
  if (const auto* registry = active_registry()) {
    return *registry;
  }
  else {
    throw TactileError{"No active registry!"};
  }
}

void DocumentModel::set_command_capacity(const usize capacity)
{
  for (auto& [id, document] : mDocuments) {
    auto& commands = document->get_history();
    commands.set_capacity(capacity);
  }
}

auto DocumentModel::is_map(const UUID& id) const -> bool
{
  return mMaps.contains(id);
}

auto DocumentModel::is_tileset(const UUID& id) const -> bool
{
  return mTilesets.contains(id);
}

auto DocumentModel::get_map(const UUID& id) -> Shared<MapDocument>
{
  if (const auto iter = mMaps.find(id); iter != mMaps.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid map document identifier!"};
  }
}

auto DocumentModel::get_map(const UUID& id) const -> Shared<const MapDocument>
{
  if (const auto iter = mMaps.find(id); iter != mMaps.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid map document identifier!"};
  }
}

auto DocumentModel::get_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  if (const auto iter = mTilesets.find(id); iter != mTilesets.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid tileset document identifier!"};
  }
}

auto DocumentModel::get_tileset(const UUID& id) const -> Shared<const TilesetDocument>
{
  if (const auto iter = mTilesets.find(id); iter != mTilesets.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid tileset document identifier!"};
  }
}

void DocumentModel::register_map(Shared<MapDocument> document)
{
  mDocuments[document->id()] = document;
  mMaps[document->id()] = std::move(document);
}

void DocumentModel::register_tileset(Shared<TilesetDocument> document)
{
  mDocuments[document->id()] = document;
  mTilesets[document->id()] = std::move(document);
}

}  // namespace tactile
