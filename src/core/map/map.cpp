#include "map.hpp"

#include <qdebug.h>

#include <cassert>

#include "algorithm.hpp"

namespace tactile::core {

map::map(row_t nRows, col_t nCols)
{
  m_layers.reserve(5);
  m_layers.emplace_back(at_least(nRows, 1_row), at_least(nCols, 1_col));
}

void map::flood(const position& pos, tile_id target, tile_id replacement)
{
  current_layer().flood(pos, target, replacement);
}

void map::set_tile(const position& pos, tile_id id)
{
  current_layer().set_tile(pos, id);
}

void map::remove_all(tile_id id)
{
  for (auto& layer : m_layers) {
    layer.remove_all(id);
  }
}

void map::select_layer(layer_id layer) noexcept
{
  if (has_layer(layer)) {
    m_activeLayer = layer;
  }
}

void map::add_layer()
{
  m_layers.emplace_back(rows(), cols());
}

void map::add_row(tile_id id)
{
  for (auto& layer : m_layers) {
    layer.add_row(id);
  }
}

void map::add_col(tile_id id)
{
  for (auto& layer : m_layers) {
    layer.add_col(id);
  }
}

void map::remove_row() noexcept
{
  if (rows() == 1_row) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_row();
  }
}

void map::remove_col() noexcept
{
  if (cols() == 1_col) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_col();
  }
}

void map::set_rows(row_t nRows)
{
  nRows = at_least(nRows, 1_row);

  if (nRows == rows()) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.set_rows(nRows);
  }
}

void map::set_cols(col_t nCols)
{
  nCols = at_least(nCols, 1_col);

  if (nCols == cols()) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.set_cols(nCols);
  }
}

void map::set_visibility(layer_id layer, bool visibility) noexcept
{
  if (has_layer(layer)) {
    m_layers[static_cast<std::size_t>(layer.get())].set_visible(visibility);
  }
}

auto map::tile_at(const position& position) const -> std::optional<tile_id>
{
  return current_layer().tile_at(position);
}

auto map::is_visible(layer_id layer) const noexcept -> bool
{
  const auto index = static_cast<std::size_t>(layer.get());
  return has_layer(layer) && m_layers[index].visible();
}

auto map::num_layers() const noexcept -> int
{
  return static_cast<int>(m_layers.size());
}

auto map::has_layer(layer_id layer) const noexcept -> bool
{
  const auto& value = layer.get();
  return value >= 0 && value < num_layers();
}

auto map::in_bounds(const position& pos) const -> bool
{
  const row_t endRow{rows()};
  const col_t endCol{cols()};

  const auto [row, col] = pos.unpack();
  return (row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol);
}

auto map::rows() const -> row_t
{
  return current_layer().rows();
}

auto map::cols() const -> col_t
{
  return current_layer().cols();
}

auto map::width() const -> int
{
  return cols().get() * m_tileSize.get();
}

auto map::height() const -> int
{
  return rows().get() * m_tileSize.get();
}

auto map::current_layer() -> layer&
{
  const auto index = static_cast<std::size_t>(m_activeLayer.get());
  assert(index >= 0 && index < m_layers.size());
  return m_layers[index];
}

auto map::current_layer() const -> const layer&
{
  const auto index = static_cast<std::size_t>(m_activeLayer.get());
  assert(index >= 0 && index < m_layers.size());
  return m_layers[index];
}

}  // namespace tactile::core
