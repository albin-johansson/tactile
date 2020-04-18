#include "tilemap.h"

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

Tilemap::Tilemap(int nRows, int nCols)
    : m_nRows{clamp_map_dimension(nRows)},
      m_nCols{clamp_map_dimension(nCols)},
      m_activeLayer{0}
{
  m_layers.emplace_back(nRows, nCols);
}

Tilemap::~Tilemap() noexcept = default;

Unique<Tilemap> Tilemap::unique(int nRows, int nCols)
{
  return std::make_unique<Tilemap>(nRows, nCols);
}

Shared<Tilemap> Tilemap::shared(int nRows, int nCols)
{
  return std::make_shared<Tilemap>(nRows, nCols);
}

void Tilemap::draw(QPainter& painter) const noexcept
{
  m_renderer.render(painter, *this);
}

void Tilemap::select(int layer) noexcept
{
  if (has_layer(layer)) {
    m_activeLayer = layer;
  }
}

void Tilemap::add_layer() noexcept
{
  m_layers.emplace_back(m_nRows, m_nCols);
}

void Tilemap::add_row(TileID id) noexcept
{
  for (auto& layer : m_layers) {
    layer.add_row(id);
  }
  ++m_nRows;
}

void Tilemap::add_col(TileID id) noexcept
{
  for (auto& layer : m_layers) {
    layer.add_col(id);
  }
  ++m_nCols;
}

void Tilemap::remove_row() noexcept
{
  if (m_nRows == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_row();
  }
  --m_nRows;
}

void Tilemap::remove_col() noexcept
{
  if (m_nCols == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_col();
  }
  --m_nCols;
}

void Tilemap::set_rows(int nRows) noexcept
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

void Tilemap::set_cols(int nCols) noexcept
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

void Tilemap::set_visibility(int layer, bool visibility) noexcept
{
  if (has_layer(layer)) {
    const auto index = static_cast<std::size_t>(layer);
    m_layers.at(index).set_visible(visibility);
  }
}

bool Tilemap::is_visible(int layer) const noexcept
{
  const auto index = static_cast<std::size_t>(layer);
  return has_layer(layer) && m_layers.at(index).visible();
}

int Tilemap::get_layer_amount() const noexcept
{
  return static_cast<int>(m_layers.size());
}

bool Tilemap::has_layer(int layer) const noexcept
{
  return layer >= 0 && layer < get_layer_amount();
}

}  // namespace tactile
