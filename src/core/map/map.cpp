#include "map.hpp"

#include <algorithm>  // max
#include <cassert>    // assert
#include <utility>    // move

#include "core/map/layers/layer_utils.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

Map::Map(const row_t nRows, const col_t nCols) : mRows{nRows}, mCols{nCols}
{
  if (mRows < 1_row || mCols < 1_col)
  {
    throw TactileError{"Invalid map dimensions!"};
  }

  mLayers.reserve(8);
  mLayers.emplace(mNextLayerId, std::make_shared<TileLayer>(mRows, mCols));

  mActiveLayer = mNextLayerId;
  ++mNextLayerId;
}

void Map::RemoveOccurrences(const tile_id id)
{
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->RemoveAll(id);
    }
  }
}

void Map::RemoveOccurrences(const tile_id first, const tile_id last)
{
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->RemoveAll(first, last);
    }
  }
}

auto Map::AddTileLayer() -> layer_id
{
  const auto id = mNextLayerId;
  AddLayer(id, MakeTileLayer());
  return id;
}

auto Map::AddObjectLayer() -> layer_id
{
  const auto id = mNextLayerId;
  AddLayer(id, MakeObjectLayer());
  return id;
}

void Map::AddLayer(const layer_id id, SharedLayer layer)
{
  // TODO what happens if dimensions mismatch?

  assert(!mLayers.contains(id));
  assert(layer);

  if (mLayers.empty())
  {
    if (const auto* tileLayer = AsTileLayer(layer))
    {
      mRows = tileLayer->GetRowCount();
      mCols = tileLayer->GetColumnCount();
    }
  }

  mLayers.emplace(id, std::move(layer));
}

auto Map::DuplicateLayer(const layer_id id) -> layer_pair&
{
  assert(mLayers.contains(id));
  const auto& layer = mLayers.at(id);

  const auto newId = mNextLayerId;
  auto copy = layer->Clone();

  auto& pair = mLayers.emplace(newId, std::move(copy));

  ++mNextLayerId;
  return pair;
}

void Map::RemoveLayer(const layer_id id)
{
  assert(mLayers.contains(id));

  if (mActiveLayer == id)
  {
    mActiveLayer.reset();
  }

  mLayers.erase(id);
}

auto Map::TakeLayer(const layer_id id) -> SharedLayer
{
  assert(mLayers.contains(id));

  if (mActiveLayer == id)
  {
    mActiveLayer.reset();
  }

  auto layer = mLayers.at(id);
  mLayers.erase(id);

  return layer;
}

void Map::SelectLayer(const layer_id id)
{
  if (mLayers.contains(id))
  {
    mActiveLayer = id;
  }
}

void Map::AddRow(const tile_id id)
{
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->AddRow(id);
    }
  }
  ++mRows;
}

void Map::AddColumn(const tile_id id)
{
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->AddColumn(id);
    }
  }
  ++mCols;
}

void Map::RemoveRow()
{
  if (GetRowCount() == 1_row)
  {
    return;
  }

  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->RemoveRow();
    }
  }

  --mRows;
}

void Map::RemoveColumn()
{
  if (GetColumnCount() == 1_col)
  {
    return;
  }

  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->RemoveColumn();
    }
  }

  --mCols;
}

void Map::SetNextLayerId(const layer_id id) noexcept
{
  assert(!HasLayer(id));
  mNextLayerId = id;
}

void Map::SetNextObjectId(const object_id id) noexcept
{
  // TODO assert that there is no object with the specified ID
  mNextObjectId = id;
}

void Map::SetRowCount(row_t nRows)
{
  nRows = std::max(nRows, 1_row);

  if (nRows == GetRowCount())
  {
    return;
  }

  mRows = nRows;
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->SetRowCount(mRows);
    }
  }
}

void Map::SetColumnCount(col_t nCols)
{
  nCols = std::max(nCols, 1_col);

  if (nCols == GetColumnCount())
  {
    return;
  }

  mCols = nCols;
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->SetColumnCount(mCols);
    }
  }
}

void Map::SetVisible(const layer_id id, const bool visible)
{
  if (auto* layer = FindLayer(id))
  {
    layer->SetVisible(visible);
  }
}

void Map::SetOpacity(const layer_id id, const float opacity)
{
  if (auto* layer = FindLayer(id))
  {
    layer->SetOpacity(opacity);
  }
}

void Map::SetName(const layer_id id, std::string name)
{
  if (auto* layer = FindLayer(id))
  {
    layer->SetName(std::move(name));
  }
}

void Map::SetTileWidth(const int tileWidth) noexcept
{
  assert(tileWidth > 0);
  mTileWidth = tileWidth;
}

void Map::SetTileHeight(const int tileHeight) noexcept
{
  assert(tileHeight > 0);
  mTileHeight = tileHeight;
}

void Map::MoveLayerDown(const layer_id id)
{
  mLayers.move_forward(id);
}

void Map::MoveLayerUp(const layer_id id)
{
  mLayers.move_backward(id);
}

auto Map::MakeTileLayer() -> Shared<TileLayer>
{
  ++mNextLayerId;
  if (!mActiveLayer)
  {
    return std::make_shared<TileLayer>(5_row, 5_col);
  }
  else
  {
    return std::make_shared<TileLayer>(GetRowCount(), GetColumnCount());
  }
}

auto Map::MakeObjectLayer() -> Shared<ObjectLayer>
{
  ++mNextLayerId;
  return std::make_shared<ObjectLayer>();
}

auto Map::IndexOf(const layer_id id) const -> Maybe<usize>
{
  return mLayers.index_of(id);
}

auto Map::GetName(const layer_id id) const -> std::string_view
{
  return GetLayer(id)->GetName();
}

auto Map::IsVisible(const layer_id id) const -> bool
{
  if (const auto* layer = FindLayer(id))
  {
    return layer->IsVisible();
  }
  else
  {
    return false;
  }
}

auto Map::GetOpacity(const layer_id id) const -> float
{
  const auto* layer = FindLayer(id);
  assert(layer);

  return layer->GetOpacity();
}

auto Map::GetLayerCount() const -> int
{
  return static_cast<int>(mLayers.size());
}

auto Map::HasLayer(const layer_id id) const -> bool
{
  return mLayers.contains(id);
}

auto Map::GetLayer(const layer_id id) const -> const SharedLayer&
{
  return mLayers.at(id);
}

auto Map::GetLayer(const layer_id id) -> SharedLayer&
{
  return mLayers.at(id);
}

auto Map::GetTileLayer(const layer_id id) -> TileLayer*
{
  return AsTileLayer(GetLayer(id));
}

auto Map::GetTileLayer(const layer_id id) const -> const TileLayer*
{
  return AsTileLayer(GetLayer(id));
}

auto Map::GetObjectLayer(const layer_id id) -> ObjectLayer*
{
  return AsObjectLayer(GetLayer(id));
}

auto Map::GetObjectLayer(const layer_id id) const -> const ObjectLayer*
{
  return AsObjectLayer(GetLayer(id));
}

auto Map::GetGroupLayer(const layer_id id) -> GroupLayer*
{
  return AsGroupLayer(GetLayer(id));
}

auto Map::GetGroupLayer(const layer_id id) const -> const GroupLayer*
{
  return AsGroupLayer(GetLayer(id));
}

auto Map::IsTileLayer(const layer_id id) const -> bool
{
  return GetLayer(id)->GetType() == LayerType::TileLayer;
}

auto Map::IsObjectLayer(const layer_id id) const -> bool
{
  return GetLayer(id)->GetType() == LayerType::ObjectLayer;
}

auto Map::InBounds(const MapPosition& pos) const -> bool
{
  const auto endRow = mRows;
  const auto endCol = mCols;
  const auto row = pos.GetRowIndex();
  const auto col = pos.GetColumnIndex();
  return (row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol);
}

auto Map::FindLayer(const layer_id id) -> ILayer*
{
  if (const auto it = mLayers.find(id); it != mLayers.end())
  {
    return it->second.get();
  }
  else
  {
    return nullptr;
  }
}

auto Map::FindLayer(const layer_id id) const -> const ILayer*
{
  if (const auto it = mLayers.find(id); it != mLayers.end())
  {
    return it->second.get();
  }
  else
  {
    return nullptr;
  }
}

}  // namespace Tactile
