#include "map.hpp"

#include <algorithm>  // max
#include <utility>    // move

#include "core/tactile_error.hpp"
#include "layers/layer_utils.hpp"
#include "utils/buffer_utils.hpp"

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
  TileLayerQuery query{mLayers};
  for (auto [layerId, layer] : query)
  {
    layer->RemoveAll(id);
  }
}

void Map::RemoveOccurrences(const tile_id first, const tile_id last)
{
  TileLayerQuery query{mLayers};
  for (auto [id, layer] : query)
  {
    layer->RemoveAll(first, last);
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
  assert(!HasLayer(id));
  assert(layer);

  if (mLayers.empty())
  {
    if (const auto* tileLayer = AsTileLayer(layer))
    {
      mRows = tileLayer->GetRowCount();
      mCols = tileLayer->GetColumnCount();
    }
  }

  if (mActiveLayer)
  {
    if (auto* groupLayer = GetGroupLayer(*mActiveLayer))
    {
      groupLayer->AddLayer(id, std::move(layer));
      return;
    }
  }

  mLayers.emplace(id, std::move(layer));
}

auto Map::DuplicateLayer(const layer_id id) -> layer_pair&
{
  assert(HasLayer(id));
  const auto layer = GetLayer(id);

  const auto newId = mNextLayerId;
  auto copy = layer->Clone();

  auto& layers = GetStorage(id);
  auto& pair = layers.emplace(newId, std::move(copy));

  ++mNextLayerId;
  return pair;
}

void Map::RemoveLayer(const layer_id id)
{
  if (mActiveLayer == id)
  {
    mActiveLayer.reset();
  }

  const auto layerWasRemoved = Tactile::RemoveLayer(mLayers, id);
  assert(layerWasRemoved);
}

auto Map::TakeLayer(const layer_id id) -> SharedLayer
{
  assert(HasLayer(id));

  if (mActiveLayer == id)
  {
    mActiveLayer.reset();
  }

  auto layer = GetLayer(id);
  RemoveLayer(id);

  return layer;
}

void Map::SelectLayer(const layer_id id)
{
  if (HasLayer(id))
  {
    mActiveLayer = id;
  }
}

void Map::AddRow(const tile_id id)
{
  TileLayerQuery query{mLayers};
  for (auto [layerId, layer] : query)
  {
    layer->AddRow(id);
  }

  ++mRows;
}

void Map::AddColumn(const tile_id id)
{
  TileLayerQuery query{mLayers};
  for (auto [layerId, layer] : query)
  {
    layer->AddColumn(id);
  }

  ++mCols;
}

void Map::RemoveRow()
{
  if (GetRowCount() == 1_row)
  {
    return;
  }

  TileLayerQuery query{mLayers};
  for (auto [id, layer] : query)
  {
    layer->RemoveRow();
  }

  --mRows;
}

void Map::RemoveColumn()
{
  if (GetColumnCount() == 1_col)
  {
    return;
  }

  TileLayerQuery query{mLayers};
  for (auto [id, layer] : query)
  {
    layer->RemoveColumn();
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

  TileLayerQuery query{mLayers};
  for (auto [id, layer] : query)
  {
    layer->SetRowCount(mRows);
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

  TileLayerQuery query{mLayers};
  for (auto [id, layer] : query)
  {
    layer->SetColumnCount(mCols);
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
  auto& layers = GetStorage(id);
  layers.move_forward(id);
}

void Map::MoveLayerUp(const layer_id id)
{
  auto& layers = GetStorage(id);
  layers.move_backward(id);
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

auto Map::MakeGroupLayer() -> Shared<GroupLayer>
{
  ++mNextLayerId;
  return std::make_shared<GroupLayer>();
}

auto Map::IndexOf(const layer_id id) const -> Maybe<usize>
{
  return GetIndex(mLayers, id);
}

auto Map::GetParent(const layer_id id) const -> Maybe<layer_id>
{
  GroupLayerQuery query{mLayers};
  for (auto [layerId, layer] : query)
  {
    // We intentionally don't use ContainsLayer here!
    if (layer->GetLayers().contains(id))
    {
      return layerId;
    }
  }

  return nothing;
}

auto Map::GetParentLayer(layer_id id) -> GroupLayer*
{
  GroupLayerQuery query{mLayers};
  for (auto [layerId, layer] : query)
  {
    // We intentionally don't use ContainsLayer here!
    if (layer->GetLayers().contains(id))
    {
      return layer;
    }
  }

  return nullptr;
}

auto Map::GetParentLayer(const layer_id id) const -> const GroupLayer*
{
  GroupLayerQuery query{mLayers};
  for (const auto [layerId, layer] : query)
  {
    // We intentionally don't use ContainsLayer here!
    if (layer->GetLayers().contains(id))
    {
      return layer;
    }
  }

  return nullptr;
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
  return GetLayer(id)->GetOpacity();
}

auto Map::GetLayerCount() const -> int
{
  return static_cast<int>(mLayers.size());
}

auto Map::HasLayer(const layer_id id) const -> bool
{
  return Tactile::FindLayer(mLayers, id) != nullptr;
}

auto Map::GetLayer(const layer_id id) -> SharedLayer
{
  return Tactile::GetLayer(mLayers, id);
}

auto Map::GetLayer(const layer_id id) const -> SharedLayer
{
  return Tactile::GetLayer(mLayers, id);
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

auto Map::IsGroupLayer(const layer_id id) const -> bool
{
  return GetLayer(id)->GetType() == LayerType::GroupLayer;
}

auto Map::CanMoveLayerDown(const layer_id id) const -> bool
{
  const auto& layers = GetStorage(id);
  return layers.index_of(id) != (layers.size() - 1);
}

auto Map::CanMoveLayerUp(const layer_id id) const -> bool
{
  const auto& layers = GetStorage(id);
  return layers.index_of(id) != 0;
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
  return Tactile::FindLayer(mLayers, id).get();
}

auto Map::FindLayer(const layer_id id) const -> const ILayer*
{
  return Tactile::FindLayer(mLayers, id).get();
}

auto Map::GetStorage(const layer_id id) -> layer_map&
{
  if (auto* parent = GetParentLayer(id))
  {
    return parent->GetLayers();
  }
  else
  {
    return mLayers;
  }
}

auto Map::GetStorage(const layer_id id) const -> const layer_map&
{
  if (const auto* parent = GetParentLayer(id))
  {
    return parent->GetLayers();
  }
  else
  {
    return mLayers;
  }
}

}  // namespace Tactile
