#include "tile_map.h"

#include <QPainter>

#include "algorithm_utils.h"
#include "tile_id.h"
#include "tile_map_renderer.h"

namespace tactile {
namespace {

[[nodiscard]] int clamp_map_dimension(int dim) noexcept
{
  return (dim < 1) ? 1 : dim;
}

}  // namespace

TileMap::TileMap(int nRows, int nCols)
    : m_nRows{clamp_map_dimension(nRows)},
      m_nCols{clamp_map_dimension(nCols)},
      m_activeLayer{0},
      m_renderer{std::make_unique<TileMapRenderer>()}
{
  m_layers.emplace_back(nRows, nCols);
}

TileMap::~TileMap() noexcept = default;

void TileMap::draw(QPainter& painter) const noexcept
{
  m_renderer->render(painter, *this);
}

void TileMap::select(int layer) noexcept
{
  if (has_layer(layer)) {
    m_activeLayer = layer;
  }
}

void TileMap::add_layer() noexcept
{
  m_layers.emplace_back(m_nRows, m_nCols);
}

void TileMap::add_row(TileID id) noexcept
{
  for (auto& layer : m_layers) {
    layer.add_row(id);
  }
  ++m_nRows;
}

void TileMap::add_col(TileID id) noexcept
{
  for (auto& layer : m_layers) {
    layer.add_col(id);
  }
  ++m_nCols;
}

void TileMap::remove_row() noexcept
{
  if (m_nRows == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_row();
  }
  --m_nRows;
}

void TileMap::remove_col() noexcept
{
  if (m_nCols == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_col();
  }
  --m_nCols;
}

void TileMap::set_rows(int nRows) noexcept
{
  nRows = clamp_map_dimension(nRows);

  if (nRows == m_nRows) {
    return;
  }

  const auto action =
      (nRows > m_nRows)
          ? [](TileLayer& layer) noexcept { layer.add_row(empty); }
          : [](TileLayer& layer) noexcept { layer.remove_row(); };

  const auto nSteps = std::abs(nRows - m_nRows);
  repeat(nSteps, [&] { for_all(m_layers, action); });

  m_nRows = nRows;
}

void TileMap::set_cols(int nCols) noexcept
{
  nCols = clamp_map_dimension(nCols);

  if (nCols == m_nCols) {
    return;
  }

  const auto action =
      (nCols > m_nCols)
          ? [](TileLayer& layer) noexcept { layer.add_col(empty); }
          : [](TileLayer& layer) noexcept { layer.remove_col(); };

  const auto nSteps = std::abs(m_nCols - nCols);
  repeat(nSteps, [&] { for_all(m_layers, action); });

  m_nCols = nCols;
}

void TileMap::set_visibility(int layer, bool visibility) noexcept
{
  if (has_layer(layer)) {
    m_layers.at(layer).set_visible(visibility);
  }
}

bool TileMap::is_visible(int layer) const noexcept
{
  return has_layer(layer) && m_layers.at(layer).visible();
}

int TileMap::get_layer_amount() const noexcept
{
  return static_cast<int>(m_layers.size());
}

bool TileMap::has_layer(int layer) const noexcept
{
  return layer >= 0 && layer < get_layer_amount();
}

}  // namespace tactile
