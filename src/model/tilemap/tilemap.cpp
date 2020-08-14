#include "tilemap.hpp"

#include "algorithm_utils.hpp"

namespace tactile::model {
namespace {

[[nodiscard]] auto clamp_map_dimension(int dim) noexcept -> int
{
  return (dim < 1) ? 1 : dim;
}

}  // namespace

tilemap::tilemap(int nRows, int nCols)
    : m_nRows{clamp_map_dimension(nRows)},
      m_nCols{clamp_map_dimension(nCols)},
      m_activeLayer{0}
{
  m_layers.emplace_back(nRows, nCols);
}

void tilemap::select(int layer) noexcept
{
  if (has_layer(layer)) {
    m_activeLayer = layer;
  }
}

void tilemap::add_layer() noexcept
{
  m_layers.emplace_back(m_nRows, m_nCols);
}

void tilemap::add_row(tile_id id) noexcept
{
  for (auto& layer : m_layers) {
    layer.add_row(id);
  }
  ++m_nRows;
}

void tilemap::add_col(tile_id id) noexcept
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

void tilemap::set_rows(int nRows) noexcept
{
  nRows = clamp_map_dimension(nRows);

  if (nRows == m_nRows) {
    return;
  }

  const auto nSteps = std::abs(nRows - m_nRows);

  if (nRows > m_nRows) {
    for (auto i = 0; i < nSteps; ++i) {
      for_all(m_layers,
              [](tile_layer& layer) noexcept { layer.add_row(empty); });
    }
  } else {
    for (auto i = 0; i < nSteps; ++i) {
      for_all(m_layers, [](tile_layer& layer) noexcept { layer.remove_row(); });
    }
  }

  m_nRows = nRows;
}

void tilemap::set_cols(int nCols) noexcept
{
  nCols = clamp_map_dimension(nCols);

  if (nCols == m_nCols) {
    return;
  }

  const auto nSteps = std::abs(m_nCols - nCols);

  if (nCols > m_nCols) {
    for (auto i = 0; i < nSteps; ++i) {
      for_all(m_layers,
              [](tile_layer& layer) noexcept { layer.add_col(empty); });
    }
  } else {
    for (auto i = 0; i < nSteps; ++i) {
      for_all(m_layers, [](tile_layer& layer) noexcept { layer.remove_col(); });
    }
  }

  m_nCols = nCols;
}

void tilemap::set_visibility(int layer, bool visibility) noexcept
{
  if (has_layer(layer)) {
    const auto index = static_cast<std::size_t>(layer);
    m_layers.at(index).set_visible(visibility);
  }
}

auto tilemap::is_visible(int layer) const noexcept -> bool
{
  const auto index = static_cast<std::size_t>(layer);
  return has_layer(layer) && m_layers.at(index).visible();
}

auto tilemap::num_layers() const noexcept -> int
{
  return static_cast<int>(m_layers.size());
}

auto tilemap::has_layer(int layer) const noexcept -> bool
{
  return layer >= 0 && layer < num_layers();
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
