#include "tile_layer.hpp"

#include <cassert>  // assert
#include <cmath>    // abs
#include <utility>  // move

#include "core/algorithms/flood_fill.hpp"
#include "core/algorithms/invoke_n.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

auto MakeTileRow(const col_t nCols, const tile_id value) -> TileRow
{
  TileRow row;
  row.reserve(nCols);
  row.assign(nCols, value);
  return row;
}

auto MakeTileMatrix(const row_t nRows, const col_t nCols) -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, MakeTileRow(nCols, empty_tile));
  return tiles;
}

TileLayer::TileLayer(const row_t nRows, const col_t nCols)
    : mDelegate{LayerType::TileLayer}
{
  if (nRows < 1_row || nCols < 1_col)
  {
    throw TactileError{"Invalid tile layer dimensions!"};
  }

  mDelegate.SetName("Tile layer");

  mTiles = MakeTileMatrix(nRows, nCols);
  assert(GetRowCount() == nRows);
  assert(GetColumnCount() == nCols);
}

void TileLayer::SetVisible(const bool visible)
{
  mDelegate.SetVisible(visible);
}

void TileLayer::SetOpacity(const float opacity)
{
  mDelegate.SetOpacity(opacity);
}

void TileLayer::SetName(std::string name)
{
  mDelegate.SetName(std::move(name));
}

auto TileLayer::IsVisible() const -> bool
{
  return mDelegate.IsVisible();
}

auto TileLayer::GetOpacity() const noexcept -> float
{
  return mDelegate.GetOpacity();
}

auto TileLayer::GetType() const -> LayerType
{
  return mDelegate.GetType();
}

auto TileLayer::Clone() const -> SharedLayer
{
  return std::make_shared<TileLayer>(*this);
}

void TileLayer::Flood(const MapPosition& origin,
                      const tile_id replacement,
                      std::vector<MapPosition>& positions)
{
  FloodFill(*this, origin, replacement, positions);
}

void TileLayer::RemoveAll(const tile_id id)
{
  const auto nRows = GetRowCount().get();
  const auto nCols = GetColumnCount().get();
  for (auto row = 0; row < nRows; ++row)
  {
    for (auto col = 0; col < nCols; ++col)
    {
      if (mTiles[row][col] == id)
      {
        mTiles[row][col] = empty_tile;
      }
    }
  }
}

void TileLayer::RemoveAll(const tile_id first, const tile_id last)
{
  const auto nRows = GetRowCount().get();
  const auto nCols = GetColumnCount().get();
  for (auto row = 0; row < nRows; ++row)
  {
    for (auto col = 0; col < nCols; ++col)
    {
      auto& id = mTiles[row][col];
      if (id >= first && id <= last)
      {
        id = empty_tile;
      }
    }
  }
}

void TileLayer::AddRow(const tile_id id)
{
  mTiles.push_back(MakeTileRow(GetColumnCount(), id));
}

void TileLayer::AddColumn(const tile_id id)
{
  for (auto& row : mTiles)
  {
    row.push_back(id);
  }
}

void TileLayer::RemoveRow() noexcept
{
  if (mTiles.size() > 1)
  {
    mTiles.pop_back();
  }
}

void TileLayer::RemoveColumn() noexcept
{
  for (auto& row : mTiles)
  {
    if (row.size() > 1)
    {
      row.pop_back();
    }
  }
}

void TileLayer::SetRowCount(const row_t nRows)
{
  assert(nRows >= 1_row);

  const auto current = GetRowCount();

  if (nRows == current)
  {
    return;
  }

  const auto diff = std::abs(current - nRows);

  if (current < nRows)
  {
    InvokeN(diff, [this] { AddRow(empty_tile); });
  }
  else
  {
    InvokeN(diff, [this]() noexcept { RemoveRow(); });
  }
}

void TileLayer::SetColumnCount(const col_t nCols)
{
  assert(nCols >= 1_col);

  const auto current = GetColumnCount();

  if (nCols == current)
  {
    return;
  }

  const auto diff = std::abs(current - nCols);

  if (current < nCols)
  {
    InvokeN(diff, [this] { AddColumn(empty_tile); });
  }
  else
  {
    InvokeN(diff, [this]() noexcept { RemoveColumn(); });
  }
}

void TileLayer::SetTile(const MapPosition& position, const tile_id id)
{
  if (InBounds(position))
  {
    mTiles[position.GetRowIndex()][position.GetColumnIndex()] = id;
  }
}

auto TileLayer::GetRowCount() const noexcept -> row_t
{
  return AsRow(mTiles.size());
}

auto TileLayer::GetColumnCount() const noexcept -> col_t
{
  assert(!mTiles.empty());
  return AsColumn(mTiles[0].size());
}

auto TileLayer::GetTileCount() const noexcept -> int
{
  return GetRowCount().get() * GetColumnCount().get();
}

auto TileLayer::GetTile(const MapPosition& position) const -> Maybe<tile_id>
{
  if (InBounds(position))
  {
    return mTiles[position.GetRowIndex()][position.GetColumnIndex()];
  }
  else
  {
    return nothing;
  }
}

auto TileLayer::InBounds(const MapPosition& position) const -> bool
{
  const auto row = position.GetRowIndex();
  return (row < mTiles.size()) && (position.GetColumnIndex() < mTiles[row].size());
}

void TileLayer::AddProperty(const std::string& name, const PropertyType type)
{
  mDelegate.AddProperty(name, type);
}

void TileLayer::AddProperty(const std::string& name, const Property& property)
{
  mDelegate.AddProperty(name, property);
}

void TileLayer::RemoveProperty(const std::string_view name)
{
  mDelegate.RemoveProperty(name);
}

void TileLayer::RenameProperty(const std::string_view oldName,
                               const std::string& newName)
{
  mDelegate.RenameProperty(oldName, newName);
}

void TileLayer::SetProperty(const std::string_view name, const Property& property)
{
  mDelegate.SetProperty(name, property);
}

void TileLayer::ChangePropertyType(const std::string_view name,
                                   const PropertyType type)
{
  mDelegate.ChangePropertyType(name, type);
}

auto TileLayer::HasProperty(const std::string_view name) const -> bool
{
  return mDelegate.HasProperty(name);
}

auto TileLayer::GetProperty(const std::string_view name) const -> const Property&
{
  return mDelegate.GetProperty(name);
}

auto TileLayer::GetProperties() const -> const PropertyMap&
{
  return mDelegate.GetProperties();
}

auto TileLayer::GetPropertyCount() const -> usize
{
  return mDelegate.GetPropertyCount();
}

auto TileLayer::GetName() const -> std::string_view
{
  return mDelegate.GetName();
}

}  // namespace Tactile
