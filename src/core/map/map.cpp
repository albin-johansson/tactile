#include "map.hpp"

#include "algorithm.hpp"
#include "layer_utils.hpp"
#include "object_layer.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::core {

Map::Map(const row_t nRows, const col_t nCols) : mRows{nRows}, mCols{nCols}
{
  if (mRows < 1_row || mCols < 1_col)
  {
    throw tactile_error{"Invalid map dimensions!"};
  }

  mLayers.reserve(5);
  mLayers.emplace(mNextLayer, std::make_shared<TileLayer>(mRows, mCols));

  mActiveLayer = mNextLayer;
  ++mNextLayer;
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

void Map::RemoveLayer(const layer_id id)
{
  Q_ASSERT(mLayers.contains(id));

  if (mActiveLayer == id)
  {
    mActiveLayer.reset();
  }

  mLayers.erase(id);
}

auto Map::TakeLayer(const layer_id id) -> shared<ILayer>
{
  Q_ASSERT(mLayers.contains(id));

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

auto Map::AddTileLayer() -> layer_id
{
  const auto id = mNextLayer;
  AddLayer(id, MakeTileLayer());
  return id;
}

auto Map::AddObjectLayer() -> layer_id
{
  const auto id = mNextLayer;
  AddLayer(id, MakeObjectLayer());
  return id;
}

void Map::AddLayer(const layer_id id, shared<ILayer> layer)
{
  // TODO what happens if dimensions mismatch?

  Q_ASSERT(!mLayers.contains(id));
  Q_ASSERT(layer);

  if (mLayers.empty())
  {
    if (const auto* tileLayer = AsTileLayer(layer))
    {
      mRows = tileLayer->RowCount();
      mCols = tileLayer->ColumnCount();
    }
  }

  mLayers.emplace(id, std::move(layer));
}

auto Map::DuplicateLayer(const layer_id id) -> layer_pair&
{
  Q_ASSERT(mLayers.contains(id));
  const auto& layer = mLayers.at(id);

  const auto newId = mNextLayer;
  auto copy = layer->Clone();

  auto& pair = mLayers.emplace(newId, std::move(copy));

  ++mNextLayer;
  return pair;
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
  if (RowCount() == 1_row)
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
  if (ColumnCount() == 1_col)
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

void Map::IncreaseTileSize()
{
  mTileSize.increase();
}

void Map::DecreaseTileSize() noexcept
{
  mTileSize.decrease();
}

void Map::ResetTileSize() noexcept
{
  mTileSize.reset();
}

void Map::SetNextLayerId(const layer_id id) noexcept
{
  Q_ASSERT(!HasLayer(id));
  mNextLayer = id;
}

void Map::SetNextObjectId(const object_id id) noexcept
{
  mNextObject = id;
}

void Map::SetRowCount(row_t nRows)
{
  nRows = AtLeast(nRows, 1_row);

  if (nRows == RowCount())
  {
    return;
  }

  mRows = nRows;
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->SetRows(mRows);
    }
  }
}

void Map::SetColumnCount(col_t nCols)
{
  nCols = AtLeast(nCols, 1_col);

  if (nCols == ColumnCount())
  {
    return;
  }

  mCols = nCols;
  for (auto& [key, layer] : mLayers)
  {
    if (auto* tileLayer = AsTileLayer(layer))
    {
      tileLayer->SetColumns(mCols);
    }
  }
}

void Map::SetVisibility(const layer_id id, const bool visible)
{
  if (auto* layer = FindLayer(id))
  {
    layer->SetVisible(visible);
  }
}

void Map::SetOpacity(const layer_id id, const double opacity)
{
  if (auto* layer = FindLayer(id))
  {
    layer->SetOpacity(opacity);
  }
}

void Map::SetName(const layer_id id, const QString& name)
{
  if (auto* layer = FindLayer(id))
  {
    layer->SetName(name);
  }
}

void Map::MoveLayerBack(const layer_id id)
{
  mLayers.move_elem_back(id);
}

void Map::MoveLayerForward(const layer_id id)
{
  mLayers.move_elem_forward(id);
}

auto Map::MakeTileLayer() -> shared<TileLayer>
{
  ++mNextLayer;
  if (!mActiveLayer)
  {
    return std::make_shared<TileLayer>(5_row, 5_col);
  }
  else
  {
    return std::make_shared<TileLayer>(RowCount(), ColumnCount());
  }
}

auto Map::MakeObjectLayer() -> shared<ObjectLayer>
{
  ++mNextLayer;
  return std::make_shared<ObjectLayer>();
}

auto Map::IndexOf(const layer_id id) const -> maybe<std::size_t>
{
  return mLayers.index_of(id);
}

auto Map::Name(const layer_id id) const -> QString
{
  return GetLayer(id)->Name();
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

auto Map::LayerCount() const noexcept -> int
{
  return static_cast<int>(mLayers.size());
}

auto Map::HasLayer(const layer_id id) const -> bool
{
  return mLayers.contains(id);
}

auto Map::InBounds(const Position& pos) const -> bool
{
  const auto endRow = mRows;
  const auto endCol = mCols;

  const auto [row, col] = pos.Unpack();
  return (row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol);
}

auto Map::RowCount() const -> row_t
{
  return mRows;
}

auto Map::ColumnCount() const -> col_t
{
  return mCols;
}

auto Map::Width() const -> int
{
  return mCols.get() * mTileSize.get();
}

auto Map::Height() const -> int
{
  return mRows.get() * mTileSize.get();
}

auto Map::ActiveLayerId() const noexcept -> maybe<layer_id>
{
  return mActiveLayer;
}

auto Map::NextLayerId() const noexcept -> layer_id
{
  return mNextLayer;
}

auto Map::CurrentTileSize() const noexcept -> int
{
  return mTileSize.get();
}

auto Map::GetLayer(const layer_id id) const -> const shared<ILayer>&
{
  return mLayers.at(id);
}

auto Map::GetLayer(const layer_id id) -> shared<ILayer>&
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

}  // namespace tactile::core
