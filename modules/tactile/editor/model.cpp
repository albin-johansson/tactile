#include "model.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/map.hpp"
#include "core/systems/animation_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/systems/viewport_system.hpp"

namespace Tactile {

void Model::Update()
{
  if (auto* registry = GetActiveRegistry()) {
    Sys::UpdateAnimations(*registry);
  }
}

void Model::OnCommandCapacityChanged(const SetCommandCapacityEvent& event)
{
  for (auto& [id, document] : mDocuments) {
    document->commands.SetCapacity(event.capacity);
  }
}

auto Model::IsClean() const -> bool
{
  return mActiveMap && mDocuments.At(*mActiveMap)->commands.IsClean();
}

auto Model::CanUndo() const -> bool
{
  return mActiveMap && mDocuments.At(*mActiveMap)->commands.CanUndo();
}

auto Model::CanRedo() const -> bool
{
  return mActiveMap && mDocuments.At(*mActiveMap)->commands.CanRedo();
}

auto Model::GetUndoText() const -> const std::string&
{
  TACTILE_ASSERT(CanUndo());
  return mDocuments.At(mActiveMap.value())->commands.GetUndoText();
}

auto Model::GetRedoText() const -> const std::string&
{
  TACTILE_ASSERT(CanRedo());
  return mDocuments.At(mActiveMap.value())->commands.GetRedoText();
}

auto Model::CanSaveDocument() const -> bool
{
  if (mActiveMap) {
    const auto& document = mDocuments.At(*mActiveMap);
    return !document->commands.IsClean();
  }
  else {
    return false;
  }
}

auto Model::CanDecreaseViewportTileSize() const -> bool
{
  if (HasActiveDocument()) {
    const auto& document = mDocuments.At(*mActiveMap);
    return Sys::CanDecreaseViewportZoom(document->registry);
  }

  return false;
}

auto Model::AddMap(Document document) -> MapID
{
  const auto id = mNextId;

  mDocuments.Emplace(id, std::make_unique<Document>(std::move(document)));
  mActiveMap = id;

  ++mNextId;
  return id;
}

auto Model::AddMap(const int tileWidth,
                   const int tileHeight,
                   const usize rows,
                   const usize columns) -> MapID
{
  TACTILE_ASSERT(tileWidth > 0);
  TACTILE_ASSERT(tileHeight > 0);

  Document document;
  document.registry = Sys::MakeRegistry();

  auto& map = document.registry.ctx<Map>();
  map.tile_width = tileWidth;
  map.tile_height = tileHeight;
  map.row_count = rows;
  map.column_count = columns;

  return AddMap(std::move(document));
}

void Model::SelectMap(const MapID id)
{
  TACTILE_ASSERT(mDocuments.Contains(id));
  mActiveMap = id;
}

void Model::RemoveMap(const MapID id)
{
  TACTILE_ASSERT(mDocuments.Contains(id));
  mDocuments.Erase(id);

  if (mActiveMap == id) {
    if (!mDocuments.IsEmpty()) {
      const auto& [mapId, map] = mDocuments.AtIndex(0);
      mActiveMap = mapId;
    }
    else {
      mActiveMap.reset();
    }
  }
}

auto Model::HasPath(const MapID id) const -> bool
{
  TACTILE_ASSERT(mDocuments.Contains(id));
  return !mDocuments.At(id)->path.empty();
}

auto Model::GetPath(const MapID id) const -> const std::filesystem::path&
{
  TACTILE_ASSERT(mDocuments.Contains(id));
  return mDocuments.At(id)->path;
}

auto Model::HasDocumentWithPath(const std::filesystem::path& path) const -> bool
{
  for (const auto& [id, document] : mDocuments) {
    if (document->path == path) {
      return true;
    }
  }

  return false;
}

auto Model::HasActiveDocument() const -> bool
{
  return mActiveMap.has_value();
}

auto Model::GetActiveDocument() -> Document*
{
  if (mActiveMap) {
    return mDocuments.At(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto Model::GetActiveDocument() const -> const Document*
{
  if (mActiveMap) {
    return mDocuments.At(*mActiveMap).get();
  }
  else {
    return nullptr;
  }
}

auto Model::GetActiveRegistry() -> entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.At(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto Model::GetActiveRegistry() const -> const entt::registry*
{
  if (mActiveMap) {
    return &mDocuments.At(*mActiveMap)->registry;
  }
  else {
    return nullptr;
  }
}

auto Model::IsStampActive() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsStampEnabled(*registry);
  }
  else {
    return false;
  }
}

auto Model::IsEraserActive() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsEraserEnabled(*registry);
  }
  else {
    return false;
  }
}

auto Model::IsBucketActive() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsBucketEnabled(*registry);
  }
  else {
    return false;
  }
}

auto Model::IsObjectSelectionActive() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsObjectSelectionEnabled(*registry);
  }
  else {
    return false;
  }
}

auto Model::IsStampPossible() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsTileLayerActive(*registry);
  }
  else {
    return false;
  }
}

auto Model::IsEraserPossible() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsTileLayerActive(*registry);
  }
  else {
    return false;
  }
}

auto Model::IsBucketPossible() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsTileLayerActive(*registry) &&
           Sys::IsSingleTileSelectedInTileset(*registry);
  }
  else {
    return false;
  }
}

auto Model::IsObjectSelectionPossible() const -> bool
{
  if (const auto* registry = GetActiveRegistry()) {
    return Sys::IsObjectLayerActive(*registry);
  }
  else {
    return false;
  }
}

}  // namespace Tactile
