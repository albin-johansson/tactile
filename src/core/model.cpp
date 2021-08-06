#include "model.hpp"

#include <cassert>  // assert
#include <utility>  // move

namespace Tactile {

Model::Model() : mTools{this}
{}

void Model::OnCommandCapacityChanged(const ChangeCommandCapacityEvent& event)
{
  for (auto& [id, document] : mDocuments)
  {
    document->SetCommandCapacity(event.capacity);
  }
}

auto Model::AddMap(Unique<MapDocument> document) -> map_id
{
  const auto id = mNextId;

  mDocuments.emplace(id, std::move(document));
  mActiveMap = id;

  ++mNextId;
  return id;
}

auto Model::AddMap(const int tileWidth, const int tileHeight) -> map_id
{
  return AddMap(std::make_unique<MapDocument>(5_row, 5_col, tileWidth, tileHeight));
}

void Model::SelectMap(const map_id id)
{
  assert(mDocuments.contains(id));
  mActiveMap = id;
}

void Model::RemoveMap(const map_id id)
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

auto Model::GetDocument(const map_id id) -> MapDocument*
{
  return mDocuments.at(id).get();
}

auto Model::GetDocument(const map_id id) const -> const MapDocument*
{
  return mDocuments.at(id).get();
}

auto Model::GetActiveDocument() -> MapDocument*
{
  return mActiveMap ? GetDocument(*mActiveMap) : nullptr;
}

auto Model::GetActiveDocument() const -> const MapDocument*
{
  return mActiveMap ? GetDocument(*mActiveMap) : nullptr;
}

auto Model::GetActiveMap() -> Map&
{
  assert(mActiveMap.has_value());
  return GetDocument(*mActiveMap)->GetMap();
}

auto Model::GetActiveMap() const -> const Map&
{
  assert(mActiveMap.has_value());
  return GetDocument(*mActiveMap)->GetMap();
}

void Model::SelectTool(const MouseToolType tool)
{
  mTools.Select(tool);
}

void Model::OnMousePressed(const MousePressedEvent& event)
{
  mTools.OnMousePressed(event);
}

void Model::OnMouseReleased(const MouseReleasedEvent& event)
{
  mTools.OnMouseReleased(event);
}

void Model::OnMouseDragged(const MouseDragEvent& event)
{
  mTools.OnMouseDragged(event);
}

auto Model::GetActiveTool() const -> MouseToolType
{
  return mTools.GetActive();
}

auto Model::IsStampActive() const -> bool
{
  return mTools.GetActive() == MouseToolType::Stamp;
}

auto Model::IsEraserActive() const -> bool
{
  return mTools.GetActive() == MouseToolType::Eraser;
}

auto Model::IsBucketActive() const -> bool
{
  return mTools.GetActive() == MouseToolType::Bucket;
}

}  // namespace Tactile
