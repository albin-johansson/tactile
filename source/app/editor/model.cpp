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

#include <algorithm>  // any_of
#include <utility>    // move

#include <entt/entity/registry.hpp>

#include "core/map_info.hpp"
#include "core/systems/animation_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "misc/assert.hpp"
#include "misc/throw.hpp"

namespace tactile {

void DocumentModel::update()
{
  if (auto* registry = active_registry()) {
    sys::update_tilesets(*registry);
    sys::update_animations(*registry);
  }
}

auto DocumentModel::add_map(Document document) -> MapID
{
  const auto id = mNextId;

  mDocuments.emplace(id, std::make_unique<Document>(std::move(document)));
  mActiveMap = id;

  ++mNextId;
  return id;
}

auto DocumentModel::add_map(const int32 tileWidth,
                            const int32 tileHeight,
                            const usize rows,
                            const usize columns) -> MapID
{
  TACTILE_ASSERT(tileWidth > 0);
  TACTILE_ASSERT(tileHeight > 0);

  Document document;
  document.registry = sys::make_document_registry();

  auto& map = document.registry.ctx<MapInfo>();
  map.tile_width = tileWidth;
  map.tile_height = tileHeight;
  map.row_count = rows;
  map.column_count = columns;

  return add_map(std::move(document));
}

void DocumentModel::select_map(const MapID id)
{
  TACTILE_ASSERT(mDocuments.contains(id));
  mActiveMap = id;
}

void DocumentModel::remove_map(const MapID id)
{
  TACTILE_ASSERT(mDocuments.contains(id));
  mDocuments.erase(id);

  if (mActiveMap == id) {
    if (!mDocuments.empty()) {
      const auto iter = mDocuments.nth(0);
      mActiveMap = iter->first;
    }
    else {
      mActiveMap.reset();
    }
  }
}

auto DocumentModel::has_path(const MapID id) const -> bool
{
  TACTILE_ASSERT(mDocuments.contains(id));
  return !mDocuments.at(id)->path.empty();
}

auto DocumentModel::get_path(const MapID id) const -> const std::filesystem::path&
{
  TACTILE_ASSERT(mDocuments.contains(id));
  return mDocuments.at(id)->path;
}

auto DocumentModel::has_document_with_path(const std::filesystem::path& path) const
    -> bool
{
  return std::any_of(mDocuments.begin(), mDocuments.end(), [&](const auto& pair) {
    return pair.second->path == path;
  });
}

auto DocumentModel::active_map_id() const -> std::optional<MapID>
{
  return mActiveMap;
}

auto DocumentModel::has_active_document() const -> bool
{
  return mActiveMap.has_value();
}

auto DocumentModel::is_save_possible() const -> bool
{
  if (mActiveMap) {
    const auto& document = mDocuments.at(*mActiveMap);
    return !document->commands.is_clean();
  }
  else {
    return false;
  }
}

auto DocumentModel::can_decrease_viewport_tile_size() const -> bool
{
  if (has_active_document()) {
    const auto& document = mDocuments.at(*mActiveMap);
    return sys::can_decrease_viewport_zoom(document->registry);
  }

  return false;
}

auto DocumentModel::active_document() -> Document*
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::active_document() const -> const Document*
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::active_registry() -> entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::active_registry() const -> const entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto DocumentModel::get_active_registry() -> entt::registry&
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap)->registry;
  }
  else {
    panic("No active registry to return!");
  }
}

auto DocumentModel::get_active_registry() const -> const entt::registry&
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap)->registry;
  }
  else {
    panic("No active registry to return!");
  }
}

void DocumentModel::set_command_capacity(const usize capacity)
{
  for (auto& [id, document] : mDocuments) {
    document->commands.set_capacity(capacity);
  }
}

auto DocumentModel::is_clean() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.is_clean();
}

auto DocumentModel::can_undo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.can_undo();
}

auto DocumentModel::can_redo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.can_redo();
}

auto DocumentModel::get_undo_text() const -> const std::string&
{
  TACTILE_ASSERT(can_undo());
  return mDocuments.at(mActiveMap.value())->commands.get_undo_text();
}

auto DocumentModel::get_redo_text() const -> const std::string&
{
  TACTILE_ASSERT(can_redo());
  return mDocuments.at(mActiveMap.value())->commands.get_redo_text();
}

auto DocumentModel::is_tool_active(const ToolType tool) const -> bool
{
  const auto* registry = active_registry();
  return registry && sys::is_tool_enabled(*registry, tool);
}

auto DocumentModel::is_tool_possible(const ToolType tool) const -> bool
{
  const auto* registry = active_registry();

  if (!registry) {
    return false;
  }

  switch (tool) {
    case ToolType::Stamp:
    case ToolType::Eraser:
      return sys::is_tile_layer_active(*registry);

    case ToolType::Bucket:
      return sys::is_tile_layer_active(*registry) &&
             sys::is_single_tile_selected_in_tileset(*registry);

    case ToolType::ObjectSelection:
    case ToolType::Rectangle:
    case ToolType::Ellipse:
    case ToolType::Point:
      return sys::is_object_layer_active(*registry);

    default:
      return false;
  }
}

}  // namespace tactile
