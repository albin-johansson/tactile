#include "map.hpp"

#include <qdebug.h>

#include <algorithm>  // iter_swap
#include <utility>    // move

#include "algorithm.hpp"
#include "tactile_error.hpp"

namespace tactile::core {

map::map(row_t nRows, col_t nCols)
{
  if (nRows < 1_row || nCols < 1_col) {
    throw tactile_error{"Invalid map dimensions!"};
  }

  m_layers.reserve(5);
  m_layers.emplace(m_nextLayer, nRows, nCols);

  m_activeLayer = m_nextLayer;
  ++m_nextLayer;
}

void map::flood(const position& origin,
                tile_id replacement,
                std::vector<position>& positions)
{
  current_layer().flood(origin, replacement, positions);
}

void map::set_tile(const position& pos, tile_id id)
{
  current_layer().set_tile(pos, id);
}

void map::remove_occurrences(tile_id id)
{
  for (auto& [key, layer] : m_layers) {
    layer.remove_all(id);
  }
}

void map::remove_active_layer()
{
  if (m_activeLayer) {
    m_layers.erase(*m_activeLayer);
    m_activeLayer.reset();
  }
}

void map::select_layer(layer_id id)
{
  if (m_layers.contains(id)) {
    m_activeLayer = id;
  }
}

auto map::add_layer() -> layer_id
{
  const auto id = m_nextLayer;

  if (!m_activeLayer) {
    m_layers.emplace(id, 5_row, 5_col);
  } else {
    m_layers.emplace(id, row_count(), col_count());
  }

  ++m_nextLayer;

  return id;
}

void map::add_layer(layer_id id, layer&& layer)
{
  Q_ASSERT(!m_layers.contains(id));
  m_layers.emplace(id, std::move(layer));
}

void map::add_row(tile_id id)
{
  for (auto& [key, layer] : m_layers) {
    layer.add_row(id);
  }
}

void map::add_col(tile_id id)
{
  for (auto& [key, layer] : m_layers) {
    layer.add_col(id);
  }
}

void map::remove_row()
{
  if (row_count() == 1_row) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer.remove_row();
  }
}

void map::remove_col()
{
  if (col_count() == 1_col) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer.remove_col();
  }
}

void map::set_next_layer_id(layer_id id) noexcept
{
  Q_ASSERT(!has_layer(id));
  m_nextLayer = id;
}

void map::set_rows(row_t nRows)
{
  nRows = at_least(nRows, 1_row);

  if (nRows == row_count()) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer.set_rows(nRows);
  }
}

void map::set_cols(col_t nCols)
{
  nCols = at_least(nCols, 1_col);

  if (nCols == col_count()) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer.set_cols(nCols);
  }
}

void map::set_visibility(layer_id id, bool visibility)
{
  if (auto* layer = find_layer(id)) {
    layer->set_visible(visibility);
  }
}

void map::set_opacity(layer_id id, double opacity)
{
  if (auto* layer = find_layer(id)) {
    layer->set_opacity(opacity);
  }
}

void map::set_name(layer_id id, const QString& name)
{
  if (auto* layer = find_layer(id)) {
    layer->set_name(name);
  }
}

void map::move_layer_back(layer_id id)
{
  m_layers.move_elem_back(id);
}

void map::move_layer_forward(layer_id id)
{
  m_layers.move_elem_forward(id);
}

auto map::tile_at(const position& position) const -> std::optional<tile_id>
{
  return current_layer().tile_at(position);
}

auto map::is_visible(layer_id id) const -> bool
{
  if (const auto* layer = find_layer(id)) {
    return layer->visible();
  } else {
    return false;
  }
}

auto map::layer_count() const noexcept -> int
{
  return static_cast<int>(m_layers.size());
}

auto map::has_layer(layer_id id) const -> bool
{
  return m_layers.contains(id);
}

auto map::in_bounds(const position& pos) const -> bool
{
  const row_t endRow{row_count()};
  const col_t endCol{col_count()};

  const auto [row, col] = pos.unpack();
  return (row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol);
}

auto map::row_count() const -> row_t
{
  return current_layer().row_count();
}

auto map::col_count() const -> col_t
{
  return current_layer().col_count();
}

auto map::tile_count() const -> int
{
  return row_count().get() * col_count().get();
}

auto map::width() const -> int
{
  return col_count().get() * m_tileSize.get();
}

auto map::height() const -> int
{
  return row_count().get() * m_tileSize.get();
}

auto map::current_layer() -> layer&
{
  return m_layers.at(m_activeLayer.value());
}

auto map::current_layer() const -> const layer&
{
  return m_layers.at(m_activeLayer.value());
}

auto map::find_layer(layer_id id) -> layer*
{
  if (const auto it = m_layers.find(id); it != m_layers.end()) {
    return &it->second;
  } else {
    return nullptr;
  }
}

auto map::find_layer(layer_id id) const -> const layer*
{
  if (const auto it = m_layers.find(id); it != m_layers.end()) {
    return &it->second;
  } else {
    return nullptr;
  }
}

}  // namespace tactile::core
