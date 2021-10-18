#include "model.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "tactile/core/map.hpp"
#include "tactile/core/systems/animation_system.hpp"
#include "tactile/core/systems/registry_factory_system.hpp"
#include "tactile/core/systems/tools/tool_system.hpp"
#include "tactile/core/systems/viewport_system.hpp"

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
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.IsClean();
}

auto Model::CanUndo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.CanUndo();
}

auto Model::CanRedo() const -> bool
{
  return mActiveMap && mDocuments.at(*mActiveMap)->commands.CanRedo();
}

auto Model::GetUndoText() const -> const std::string&
{
  assert(CanUndo());
  return mDocuments.at(mActiveMap.value())->commands.GetUndoText();
}

auto Model::GetRedoText() const -> const std::string&
{
  assert(CanRedo());
  return mDocuments.at(mActiveMap.value())->commands.GetRedoText();
}

auto Model::CanSaveDocument() const -> bool
{
  if (mActiveMap) {
    const auto& document = mDocuments.at(*mActiveMap);
    return !document->commands.IsClean();
  }
  else {
    return false;
  }
}

auto Model::CanDecreaseViewportTileSize() const -> bool
{
  if (HasActiveDocument()) {
    const auto& document = mDocuments.at(*mActiveMap);
    return Sys::CanDecreaseViewportZoom(document->registry);
  }

  return false;
}

auto Model::AddMap(Document document) -> MapID
{
  const auto id = mNextId;

  mDocuments.emplace(id, std::make_unique<Document>(std::move(document)));
  mActiveMap = id;

  ++mNextId;
  return id;
}

auto Model::AddMap(const int tileWidth, const int tileHeight) -> MapID
{
  assert(tileWidth > 0);
  assert(tileHeight > 0);

  Document document;
  document.registry = Sys::MakeRegistry();

  auto& map = document.registry.ctx<Map>();
  map.tile_width = tileWidth;
  map.tile_height = tileHeight;

  return AddMap(std::move(document));
}

void Model::SelectMap(const MapID id)
{
  assert(mDocuments.contains(id));
  mActiveMap = id;
}

void Model::RemoveMap(const MapID id)
{
  assert(mDocuments.contains(id));
  mDocuments.erase(id);

  if (mActiveMap == id) {
    if (!mDocuments.empty()) {
      mActiveMap = mDocuments.at_index(0).first;
    }
    else {
      mActiveMap.reset();
    }
  }
}

auto Model::GetPath(const MapID id) const -> const std::filesystem::path&
{
  assert(mDocuments.contains(id));
  return mDocuments.at(id)->path;
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

}  // namespace Tactile
