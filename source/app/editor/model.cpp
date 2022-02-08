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
  if (auto* registry = GetActiveRegistry()) {
    sys::update_tilesets(*registry);
    sys::update_animations(*registry);
  }
}

auto Model::AddMap(Document document) -> map_id
{
  const auto id = mNextId;

  mDocuments.emplace(id, std::make_unique<Document>(std::move(document)));
  mActiveMap = id;

  ++mNextId;
  return id;
}

auto Model::AddMap(const int tileWidth,
                   const int tileHeight,
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

  return AddMap(std::move(document));
}

void Model::SelectMap(const map_id id)
{
  TACTILE_ASSERT(mDocuments.contains(id));
  mActiveMap = id;
}

void Model::RemoveMap(const map_id id)
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

auto Model::HasPath(const map_id id) const -> bool
{
  TACTILE_ASSERT(mDocuments.contains(id));
  return !mDocuments.at(id)->path.empty();
}

auto Model::GetPath(const map_id id) const -> const std::filesystem::path&
{
  TACTILE_ASSERT(mDocuments.contains(id));
  return mDocuments.at(id)->path;
}

auto Model::HasDocumentWithPath(const std::filesystem::path& path) const -> bool
{
  return std::any_of(mDocuments.begin(), mDocuments.end(), [&](const auto& pair) {
    return pair.second->path == path;
  });
}

auto Model::GetActiveMapId() const -> maybe<map_id>
{
  return mActiveMap;
}

auto Model::HasActiveDocument() const -> bool
{
  return mActiveMap.has_value();
}

auto Model::CanSaveDocument() const -> bool
{
  if (mActiveMap) {
    const auto& document = mDocuments.at(*mActiveMap);
    return !document->commands.is_clean();
  }
  else {
    return false;
  }
}

auto Model::CanDecreaseViewportTileSize() const -> bool
{
  if (HasActiveDocument()) {
    const auto& document = mDocuments.at(*mActiveMap);
    return sys::CanDecreaseViewportZoom(document->registry);
  }

  return false;
}

auto Model::GetActiveDocument() -> Document*
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto Model::GetActiveDocument() const -> const Document*
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto Model::GetActiveRegistry() -> entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto Model::GetActiveRegistry() const -> const entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto Model::GetActiveRegistryRef() -> entt::registry&
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap)->registry;
  }
  else {
    throw_traced(tactile_error{"No active registry to return!"});
  }
}

auto Model::GetActiveRegistryRef() const -> const entt::registry&
{
  if (mActiveMap) {
    return mDocuments.at(*mActiveMap)->registry;
  }
  else {
    throw_traced(tactile_error{"No active registry to return!"});
  }
}

void Model::SetCommandCapacity(const usize capacity)
{
  for (auto& [id, document] : mDocuments) {
    document->commands.set_capacity(capacity);
  }
}

auto Model::IsClean() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.is_clean();
}

auto Model::CanUndo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.can_undo();
}

auto Model::CanRedo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.can_redo();
}

auto Model::GetUndoText() const -> const std::string&
{
  TACTILE_ASSERT(CanUndo());
  return mDocuments.at(mActiveMap.value())->commands.get_undo_text();
}

auto Model::GetRedoText() const -> const std::string&
{
  TACTILE_ASSERT(CanRedo());
  return mDocuments.at(mActiveMap.value())->commands.get_redo_text();
}

auto Model::is_tool_active(const tool_type tool) const -> bool
{
  const auto* registry = GetActiveRegistry();
  return registry && sys::is_tool_enabled(*registry, tool);
}

auto Model::is_tool_possible(const tool_type tool) const -> bool
{
  const auto* registry = GetActiveRegistry();

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
