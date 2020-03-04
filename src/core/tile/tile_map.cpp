#include "tile_map.h"

#include <QPainter>

#include "algorithm_utils.h"
#include "tile_id.h"

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
      m_activeLayer{0}
{
  m_layers.emplace_back(nRows, nCols);
}

TileMap::~TileMap() noexcept = default;

void TileMap::draw(QPainter& painter) const noexcept
{
  QPen pen;
  pen.setColor(Qt::black);
  pen.setWidth(1);

  painter.setPen(pen);

  for (int r = 0; r < m_nRows; ++r) {
    for (int c = 0; c < m_nCols; ++c) {
      painter.fillRect(c * 50, r * 50, 50, 50, Qt::green);
      painter.drawRect(c * 50, r * 50, 50, 50);
    }
  }
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
