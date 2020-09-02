#include "tilemap.hpp"

#include "algorithm.hpp"

namespace tactile::model {

tilemap::tilemap(int nRows, int nCols)
    : m_nRows{at_least(nRows, 1)}, m_nCols{at_least(nCols, 1)}
{
  m_layers.emplace_back(nRows, nCols);
}

void tilemap::select(layer_id layer) noexcept
{
  if (has_layer(layer)) {
    m_activeLayer = layer;
  }
}

void tilemap::add_layer()
{
  m_layers.emplace_back(m_nRows, m_nCols);
}

void tilemap::add_row(tile_id id)
{
  for (auto& layer : m_layers) {
    layer.add_row(id);
  }
  ++m_nRows;
}

void tilemap::add_col(tile_id id)
{
  for (auto& layer : m_layers) {
    layer.add_col(id);
  }
  ++m_nCols;
}

void tilemap::remove_row() noexcept
{
  if (m_nRows == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_row();
  }
  --m_nRows;
}

void tilemap::remove_col() noexcept
{
  if (m_nCols == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_col();
  }
  --m_nCols;
}

void tilemap::set_rows(int nRows)
{
  nRows = at_least(nRows, 1);

  if (nRows == m_nRows) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.set_rows(nRows);
  }

  m_nRows = nRows;
}

void tilemap::set_cols(int nCols)
{
  nCols = at_least(nCols, 1);

  if (nCols == m_nCols) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.set_cols(nCols);
  }

  m_nCols = nCols;
}

void tilemap::set_visibility(layer_id layer, bool visibility) noexcept
{
  if (has_layer(layer)) {
    m_layers[static_cast<std::size_t>(layer.get())].set_visible(visibility);
  }
}

auto tilemap::is_visible(layer_id layer) const noexcept -> bool
{
  const auto index = static_cast<std::size_t>(layer.get());
  return has_layer(layer) && m_layers[index].visible();
}

auto tilemap::num_layers() const noexcept -> int
{
  return static_cast<int>(m_layers.size());
}

auto tilemap::has_layer(layer_id layer) const noexcept -> bool
{
  const auto& value = layer.get();
  return value >= 0 && value < num_layers();
}

auto tilemap::rows() const noexcept -> int
{
  return m_nRows;
}

auto tilemap::cols() const noexcept -> int
{
  return m_nCols;
}

auto tilemap::width() const noexcept -> int
{
  return m_nCols * m_tileSize.get();
}

auto tilemap::height() const noexcept -> int
{
  return m_nRows * m_tileSize.get();
}

auto tilemap::get_tile_size() noexcept -> tile_size&
{
  return m_tileSize;
}

auto tilemap::get_tile_size() const noexcept -> const tile_size&
{
  return m_tileSize;
}

auto tilemap::begin() const noexcept -> const_iterator
{
  return m_layers.begin();
}

auto tilemap::end() const noexcept -> const_iterator
{
  return m_layers.end();
}

}  // namespace tactile::model
