#include "tile_layer.hpp"

#include <cassert>  // assert
#include <cmath>    // abs
#include <utility>  // move

#include "algorithm.hpp"
#include "flood_fill.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::core {
namespace {

[[nodiscard]] auto create_row(const col_t nCols, const tile_id value = empty)
    -> std::vector<tile_id>
{
  std::vector<tile_id> row;
  row.reserve(nCols.get());
  row.assign(nCols.get(), value);
  return row;
}

}  // namespace

tile_layer::tile_layer(const row_t nRows, const col_t nCols)
    : m_layerDelegate{layer_type::tile_layer}
{
  if (nRows < 1_row || nCols < 1_col) {
    throw tactile_error{"Invalid tile_layer dimensions!"};
  }
  m_layerDelegate.set_name(TACTILE_QSTRING(u"Tile layer"));

  m_tiles.reserve(nRows.get());
  m_tiles.assign(nRows.get(), create_row(nCols));

  assert(row_count() == nRows);
  assert(col_count() == nCols);
}

void tile_layer::flood(const position& origin,
                       const tile_id replacement,
                       std::vector<position>& positions)
{
  flood_fill(*this, origin, replacement, positions);
}

void tile_layer::remove_all(const tile_id id)
{
  const auto nRows = row_count().get();
  const auto nCols = col_count().get();
  for (auto r = 0; r < nRows; ++r) {
    for (auto c = 0; c < nCols; ++c) {
      if (m_tiles[r][c] == id) {
        m_tiles[r][c] = empty;
      }
    }
  }
}

void tile_layer::add_row(const tile_id id)
{
  m_tiles.push_back(create_row(col_count(), id));
}

void tile_layer::add_col(const tile_id id)
{
  for (auto& row : m_tiles) {
    row.push_back(id);
  }
}

void tile_layer::remove_row() noexcept
{
  if (m_tiles.size() > 1) {
    m_tiles.pop_back();
  }
}

void tile_layer::remove_col() noexcept
{
  for (auto& row : m_tiles) {
    if (row.size() > 1) {
      row.pop_back();
    }
  }
}

void tile_layer::set_rows(const row_t nRows)
{
  assert(nRows >= 1_row);

  const auto current = row_count();

  if (nRows == current) {
    return;
  }

  const auto diff = std::abs(current.get() - nRows.get());

  if (current < nRows) {
    invoke_n(diff, [this] {
      add_row(empty);
    });
  } else {
    invoke_n(diff, [this]() noexcept {
      remove_row();
    });
  }
}

void tile_layer::set_cols(const col_t nCols)
{
  assert(nCols >= 1_col);

  const auto current = col_count();

  if (nCols == current) {
    return;
  }

  const auto diff = std::abs(current.get() - nCols.get());

  if (current < nCols) {
    invoke_n(diff, [this] {
      add_col(empty);
    });
  } else {
    invoke_n(diff, [this]() noexcept {
      remove_col();
    });
  }
}

void tile_layer::set_tile(const position& pos, const tile_id id) noexcept
{
  if (in_bounds(pos)) {
    m_tiles[pos.row_index()][pos.col_index()] = id;
  }
}

void tile_layer::set_opacity(const double opacity)
{
  m_layerDelegate.set_opacity(opacity);
}

void tile_layer::set_name(QString name)
{
  m_layerDelegate.set_name(std::move(name));
}

void tile_layer::set_visible(const bool visible) noexcept
{
  m_layerDelegate.set_visible(visible);
}

auto tile_layer::type() const -> layer_type
{
  return m_layerDelegate.type();
}

auto tile_layer::clone() const -> shared_layer
{
  auto copy = *this;
  return std::make_shared<tile_layer>(std::move(copy));
}

auto tile_layer::row_count() const noexcept -> row_t
{
  return row_t{static_cast<int>(m_tiles.size())};
}

auto tile_layer::col_count() const noexcept -> col_t
{
  assert(!m_tiles.empty());
  return col_t{static_cast<int>(m_tiles[0].size())};
}

auto tile_layer::tile_count() const noexcept -> int
{
  return row_count().get() * col_count().get();
}

auto tile_layer::tile_at(const position& pos) const -> maybe<tile_id>
{
  if (in_bounds(pos)) {
    return m_tiles[pos.row_index()][pos.col_index()];
  } else {
    return std::nullopt;
  }
}

auto tile_layer::in_bounds(const position& pos) const noexcept -> bool
{
  const auto row = pos.row_index();
  return (row < m_tiles.size()) && (pos.col_index() < m_tiles[row].size());
}

auto tile_layer::name() const -> const QString&
{
  return m_layerDelegate.name();
}

auto tile_layer::opacity() const noexcept -> double
{
  return m_layerDelegate.opacity();
}

auto tile_layer::visible() const noexcept -> bool
{
  return m_layerDelegate.visible();
}

void tile_layer::notify_property_added(const QString& name)
{}

void tile_layer::notify_property_removed(const QString& name)
{}

void tile_layer::add_property(const QString& name, const property::type type)
{
  m_propertyDelegate.add_property(name, type);
}

void tile_layer::remove_property(const QString& name)
{
  m_propertyDelegate.remove_property(name);
}

void tile_layer::rename_property(const QString& oldName, const QString& newName)
{
  m_propertyDelegate.rename_property(oldName, newName);
}

void tile_layer::set_property(const QString& name, const property& property)
{
  m_propertyDelegate.set_property(name, property);
}

auto tile_layer::get_property(const QString& name) const -> const property&
{
  return m_propertyDelegate.get_property(name);
}

auto tile_layer::get_property(const QString& name) -> property&
{
  return m_propertyDelegate.get_property(name);
}

auto tile_layer::property_count() const noexcept -> int
{
  return m_propertyDelegate.property_count();
}

auto tile_layer::properties() const -> const property_map&
{
  return m_propertyDelegate.properties();
}

}  // namespace tactile::core
