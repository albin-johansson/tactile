#include "model.hpp"

#include <algorithm>  // any_of
#include <utility>    // move

#include "core/map.hpp"
#include "core/systems/animation_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "misc/assert.hpp"
#include "misc/throw.hpp"

namespace tactile {

void Model::Update()
{
  if (auto* registry = active_registry()) {
    sys::update_tilesets(*registry);
    sys::update_animations(*registry);
  }
}

auto Model::add_map(Document document) -> map_id
{
  const auto id = mNextId;

  mDocuments.emplace(id, std::make_unique<Document>(std::move(document)));
  mActiveMap = id;

  ++mNextId;
  return id;
}

auto Model::add_map(const int32 tileWidth,
                    const int32 tileHeight,
                    const usize rows,
                    const usize columns) -> map_id
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

void Model::select_map(const map_id id)
{
  TACTILE_ASSERT(mDocuments.contains(id));
  mActiveMap = id;
}

void Model::remove_map(const map_id id)
{
  TACTILE_ASSERT(mDocuments.contains(id));
  mDocuments.erase(id);

  if (mActiveMap == id) {
    if (!mDocuments.empty()) {
      const auto& [mapId, map] = mDocuments.at_index(0);
      mActiveMap = mapId;
    }
    else {
      mActiveMap.reset();
    }
  }
}

auto Model::has_path(const map_id id) const -> bool
{
  TACTILE_ASSERT(mDocuments.contains(id));
  return !mDocuments.at(id)->path.empty();
}

auto Model::get_path(const map_id id) const -> const std::filesystem::path&
{
  TACTILE_ASSERT(mDocuments.contains(id));
  return mDocuments.at(id)->path;
}

auto Model::has_document_with_path(const std::filesystem::path& path) const -> bool
{
  return std::any_of(mDocuments.begin(), mDocuments.end(), [&](const auto& pair) {
    return pair.second->path == path;
  });
}

auto Model::active_map_id() const -> maybe<map_id>
{
  return mActiveMap;
}

auto Model::has_active_document() const -> bool
{
  return mActiveMap.has_value();
}

auto Model::is_save_possible() const -> bool
{
  if (mActiveMap) {
    const auto& document = mDocuments.at(*mActiveMap);
    return !document->commands.is_clean();
  }
  else {
    return false;
  }
}

auto Model::can_decrease_viewport_tile_size() const -> bool
{
  if (has_active_document()) {
    const auto& document = mDocuments.at(*mActiveMap);
    return sys::CanDecreaseViewportZoom(document->registry);
  }

  return false;
}

auto Model::active_document() -> Document*
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto Model::active_document() const -> const Document*
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto Model::active_registry() -> entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto Model::active_registry() const -> const entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto Model::get_active_registry() -> entt::registry&
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap)->registry;
  }
  else {
    throw_traced(tactile_error{"No active registry to return!"});
  }
}

auto Model::get_active_registry() const -> const entt::registry&
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap)->registry;
  }
  else {
    throw_traced(tactile_error{"No active registry to return!"});
  }
}

void Model::set_command_capacity(const usize capacity)
{
  for (auto& [id, document] : mDocuments) {
    document->commands.set_capacity(capacity);
  }
}

auto Model::is_clean() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.is_clean();
}

auto Model::can_undo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.can_undo();
}

auto Model::can_redo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.can_redo();
}

auto Model::get_undo_text() const -> const std::string&
{
  TACTILE_ASSERT(can_undo());
  return mDocuments.at(mActiveMap.value())->commands.get_undo_text();
}

auto Model::get_redo_text() const -> const std::string&
{
  TACTILE_ASSERT(can_redo());
  return mDocuments.at(mActiveMap.value())->commands.get_redo_text();
}

auto Model::is_tool_active(const tool_type tool) const -> bool
{
  const auto* registry = active_registry();
  return registry && sys::is_tool_enabled(*registry, tool);
}

auto Model::is_tool_possible(const tool_type tool) const -> bool
{
  const auto* registry = active_registry();

  if (!registry) {
    return false;
  }

  switch (tool) {
    case tool_type::stamp:
    case tool_type::eraser:
      return sys::is_tile_layer_active(*registry);

    case tool_type::bucket:
      return sys::is_tile_layer_active(*registry) &&
             sys::is_single_tile_selected_in_tileset(*registry);

    case tool_type::object_selection:
    case tool_type::rectangle:
    case tool_type::ellipse:
    case tool_type::point:
      return sys::is_object_layer_active(*registry);

    default:
      return false;
  }
}

}  // namespace tactile
