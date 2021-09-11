#include "model.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "systems/animation_system.hpp"
#include "systems/registry_factory_system.hpp"
#include "systems/tileset_system.hpp"
#include "systems/tool_system.hpp"
#include "systems/viewport_system.hpp"

namespace Tactile {

void Model::Update()
{
  if (auto* registry = GetActiveRegistry())
  {
    Sys::UpdateAnimations(*registry);
  }
}

void Model::OnCommandCapacityChanged(const ChangeCommandCapacityEvent& event)
{
  for (auto& [id, document] : mDocuments)
  {
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
  if (mActiveMap)
  {
    const auto& document = mDocuments.at(*mActiveMap);
    return !document->commands.IsClean();
  }
  else
  {
    return false;
  }
}

auto Model::CanDecreaseViewportTileSize() const -> bool
{
  if (HasActiveDocument())
  {
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
  Document document;
  document.registry = Sys::MakeRegistry();
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

  if (mActiveMap == id)
  {
    if (!mDocuments.empty())
    {
      mActiveMap = mDocuments.at_index(0).first;
    }
    else
    {
      mActiveMap.reset();
    }
  }
}

auto Model::HasActiveDocument() const -> bool
{
  return mActiveMap.has_value();
}

auto Model::GetActiveDocument() -> Document*
{
  if (mActiveMap)
  {
    return mDocuments.at(*mActiveMap).get();
  }
  else
  {
    return nullptr;
  }
}

auto Model::GetActiveDocument() const -> const Document*
{
  if (mActiveMap)
  {
    return mDocuments.at(*mActiveMap).get();
  }
  else
  {
    return nullptr;
  }
}

auto Model::GetActiveRegistry() -> entt::registry*
{
  if (mActiveMap)
  {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else
  {
    return nullptr;
  }
}

auto Model::GetActiveRegistry() const -> const entt::registry*
{
  if (mActiveMap)
  {
    return &mDocuments.at(*mActiveMap)->registry;
  }
  else
  {
    return nullptr;
  }
}

auto Model::IsStampActive() const -> bool
{
  if (const auto* registry = GetActiveRegistry())
  {
    return Sys::IsStampEnabled(*registry);
  }
  else
  {
    return false;
  }
}

auto Model::IsEraserActive() const -> bool
{
  if (const auto* registry = GetActiveRegistry())
  {
    return Sys::IsEraserEnabled(*registry);
  }
  else
  {
    return false;
  }
}

auto Model::IsBucketActive() const -> bool
{
  if (const auto* registry = GetActiveRegistry())
  {
    return Sys::IsBucketEnabled(*registry);
  }
  else
  {
    return false;
  }
}

}  // namespace Tactile
