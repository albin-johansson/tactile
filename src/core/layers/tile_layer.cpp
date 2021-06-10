#include "tile_layer.hpp"

#include <cassert>  // assert
#include <cmath>    // abs
#include <utility>  // move

#include "algorithm.hpp"
#include "flood_fill.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::core {

[[nodiscard]] auto MakeTileRow(const col_t nCols, const tile_id value)
    -> tile_row
{
  tile_row row;
  row.reserve(nCols);
  row.assign(nCols, value);
  return row;
}

[[nodiscard]] auto MakeTileMatrix(const row_t nRows, const col_t nCols)
    -> tile_matrix
{
  tile_matrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, MakeTileRow(nCols));
  return tiles;
}

TileLayer::TileLayer(const row_t nRows, const col_t nCols)
    : mDelegate{LayerType::tile_layer}
{
  if (nRows < 1_row || nCols < 1_col)
  {
    throw tactile_error{"Invalid tile_layer dimensions!"};
  }
  mDelegate.SetName(TACTILE_QSTRING(u"Tile layer"));

  mTiles = MakeTileMatrix(nRows, nCols);
  assert(RowCount() == nRows);
  assert(ColumnCount() == nCols);
}

void TileLayer::Flood(const position& origin,
                      const tile_id replacement,
                      std::vector<position>& positions)
{
  FloodFill(*this, origin, replacement, positions);
}

void TileLayer::RemoveAll(const tile_id id)
{
  const auto nRows = RowCount().get();
  const auto nCols = ColumnCount().get();
  for (auto r = 0; r < nRows; ++r)
  {
    for (auto c = 0; c < nCols; ++c)
    {
      if (mTiles[r][c] == id)
      {
        mTiles[r][c] = empty;
      }
    }
  }
}

void TileLayer::RemoveAll(const tile_id first, const tile_id last)
{
  const auto nRows = RowCount().get();
  const auto nCols = ColumnCount().get();
  for (auto r = 0; r < nRows; ++r)
  {
    for (auto c = 0; c < nCols; ++c)
    {
      auto& id = mTiles[r][c];
      if (id >= first && id <= last)
      {
        id = empty;
      }
    }
  }
}

void TileLayer::AddRow(const tile_id id)
{
  mTiles.push_back(MakeTileRow(ColumnCount(), id));
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

void TileLayer::SetRows(const row_t nRows)
{
  assert(nRows >= 1_row);

  const auto current = RowCount();

  if (nRows == current)
  {
    return;
  }

  const auto diff = std::abs(current - nRows);

  if (current < nRows)
  {
    InvokeN(diff, [this] { AddRow(empty); });
  }
  else
  {
    InvokeN(diff, [this]() noexcept { RemoveRow(); });
  }
}

void TileLayer::SetColumns(const col_t nCols)
{
  assert(nCols >= 1_col);

  const auto current = ColumnCount();

  if (nCols == current)
  {
    return;
  }

  const auto diff = std::abs(current - nCols);

  if (current < nCols)
  {
    InvokeN(diff, [this] { AddColumn(empty); });
  }
  else
  {
    InvokeN(diff, [this]() noexcept { RemoveColumn(); });
  }
}

void TileLayer::SetTile(const position& pos, const tile_id id) noexcept
{
  if (InBounds(pos))
  {
    mTiles[pos.row_index()][pos.col_index()] = id;
  }
}

void TileLayer::SetOpacity(const double opacity)
{
  mDelegate.SetOpacity(opacity);
}

void TileLayer::SetName(QString name)
{
  mDelegate.SetName(std::move(name));
}

void TileLayer::SetVisible(const bool visible) noexcept
{
  mDelegate.SetVisible(visible);
}

auto TileLayer::Type() const -> LayerType
{
  return mDelegate.Type();
}

auto TileLayer::Clone() const -> shared<ILayer>
{
  return std::make_shared<TileLayer>(*this);
}

auto TileLayer::RowCount() const noexcept -> row_t
{
  return row_t{static_cast<int>(mTiles.size())};
}

auto TileLayer::ColumnCount() const noexcept -> col_t
{
  assert(!mTiles.empty());
  return col_t{static_cast<int>(mTiles[0].size())};
}

auto TileLayer::TileCount() const noexcept -> int
{
  return RowCount().get() * ColumnCount().get();
}

auto TileLayer::TileAt(const position& pos) const -> maybe<tile_id>
{
  if (InBounds(pos))
  {
    return mTiles[pos.row_index()][pos.col_index()];
  }
  else
  {
    return std::nullopt;
  }
}

auto TileLayer::InBounds(const position& pos) const noexcept -> bool
{
  const auto row = pos.row_index();
  return (row < mTiles.size()) && (pos.col_index() < mTiles[row].size());
}

auto TileLayer::Name() const -> const QString&
{
  return mDelegate.Name();
}

auto TileLayer::Opacity() const noexcept -> double
{
  return mDelegate.Opacity();
}

auto TileLayer::IsVisible() const noexcept -> bool
{
  return mDelegate.Visible();
}

void TileLayer::add_property(const QString& name, const property_type type)
{
  mDelegate.AddProperty(name, type);
}

void TileLayer::add_property(const QString& name, const property& property)
{
  mDelegate.AddProperty(name, property);
}

void TileLayer::remove_property(const QString& name)
{
  mDelegate.RemoveProperty(name);
}

void TileLayer::rename_property(const QString& oldName, const QString& newName)
{
  mDelegate.RenameProperty(oldName, newName);
}

void TileLayer::set_property(const QString& name, const property& property)
{
  mDelegate.SetProperty(name, property);
}

void TileLayer::change_property_type(const QString& name,
                                     const core::property_type type)
{
  mDelegate.ChangePropertyType(name, type);
}

auto TileLayer::get_property(const QString& name) const -> const property&
{
  return mDelegate.GetProperty(name);
}

auto TileLayer::get_property(const QString& name) -> property&
{
  return mDelegate.GetProperty(name);
}

auto TileLayer::has_property(const QString& name) const -> bool
{
  return mDelegate.HasProperty(name);
}

auto TileLayer::property_count() const noexcept -> int
{
  return mDelegate.PropertyCount();
}

auto TileLayer::properties() const -> const property_map&
{
  return mDelegate.Properties();
}

}  // namespace tactile::core
