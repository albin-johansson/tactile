#include "model.hpp"

#include <cassert>  // assert

namespace tactile {

Model::Model() = default;

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

}  // namespace tactile
