#include "model.hpp"

#include <cassert>  // assert

namespace Tactile {

Model::Model() : mTools{this}
{}

auto Model::AddMap() -> map_id
{
  const auto id = mNextId;

  mDocuments.emplace(id, std::make_unique<MapDocument>(5_row, 5_col));
  mActiveMap = id;

  ++mNextId;
  return id;
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

}  // namespace Tactile
