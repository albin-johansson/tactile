#include "map.hpp"

#include <qdebug.h>

#include <algorithm>  // iter_swap
#include <utility>    // move

#include "algorithm.hpp"
#include "tactile_error.hpp"

namespace tactile::core {

map::map(const row_t nRows, const col_t nCols)
{
  if (nRows < 1_row || nCols < 1_col) {
    throw tactile_error{"Invalid map dimensions!"};
  }

  m_layers.reserve(5);
  m_layers.emplace(m_nextLayer, std::make_shared<tile_layer>(nRows, nCols));

  m_activeLayer = m_nextLayer;
  ++m_nextLayer;
}

void map::flood(const position& origin,
                const tile_id replacement,
                std::vector<position>& positions)
{
  current_layer().flood(origin, replacement, positions);
}

void map::set_tile(const position& pos, const tile_id id)
{
  current_layer().set_tile(pos, id);
}

void map::remove_occurrences(const tile_id id)
{
  for (auto& [key, layer] : m_layers) {
    layer->remove_all(id);
  }
}

void map::remove_layer(const layer_id id)
{
  Q_ASSERT(m_layers.contains(id));

  if (m_activeLayer == id) {
    m_activeLayer.reset();
  }

  m_layers.erase(id);
}

auto map::take_layer(const layer_id id) -> std::shared_ptr<tile_layer>
{
  Q_ASSERT(m_layers.contains(id));

  if (m_activeLayer == id) {
    m_activeLayer.reset();
  }

  auto layer = m_layers.at(id);
  m_layers.erase(id);

  return layer;
}

void map::select_layer(const layer_id id)
{
  if (m_layers.contains(id)) {
    m_activeLayer = id;
  }
}

auto map::add_layer() -> layer_id
{
  const auto id = m_nextLayer;
  m_layers.emplace(id, make_layer());
  return id;
}

void map::add_layer(const layer_id id, std::shared_ptr<tile_layer> layer)
{
  Q_ASSERT(!m_layers.contains(id));
  Q_ASSERT(layer);
  m_layers.emplace(id, std::move(layer));
}

auto map::duplicate_layer(const layer_id id)
    -> std::pair<layer_id, std::shared_ptr<tile_layer>>&
{
  Q_ASSERT(m_layers.contains(id));
  const auto& layer = m_layers.at(id);

  const auto newId = m_nextLayer;
  auto copy = layer;

  auto& pair = m_layers.emplace(newId, std::move(copy));

  ++m_nextLayer;
  return pair;
}

void map::add_row(const tile_id id)
{
  for (auto& [key, layer] : m_layers) {
    layer->add_row(id);
  }
}

void map::add_col(const tile_id id)
{
  for (auto& [key, layer] : m_layers) {
    layer->add_col(id);
  }
}

void map::remove_row()
{
  if (row_count() == 1_row) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer->remove_row();
  }
}

void map::remove_col()
{
  if (col_count() == 1_col) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer->remove_col();
  }
}

void map::set_next_layer_id(const layer_id id) noexcept
{
  Q_ASSERT(!has_layer(id));
  m_nextLayer = id;
}

void map::set_row_count(row_t nRows)
{
  nRows = at_least(nRows, 1_row);

  if (nRows == row_count()) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer->set_rows(nRows);
  }
}

void map::set_col_count(col_t nCols)
{
  nCols = at_least(nCols, 1_col);

  if (nCols == col_count()) {
    return;
  }

  for (auto& [key, layer] : m_layers) {
    layer->set_cols(nCols);
  }
}

void map::set_visibility(const layer_id id, const bool visible)
{
  if (auto* layer = find_layer(id)) {
    layer->set_visible(visible);
  }
}

void map::set_opacity(const layer_id id, const double opacity)
{
  if (auto* layer = find_layer(id)) {
    layer->set_opacity(opacity);
  }
}

void map::set_name(const layer_id id, const QString& name)
{
  if (auto* layer = find_layer(id)) {
    layer->set_name(name);
  }
}

void map::move_layer_back(const layer_id id)
{
  m_layers.move_elem_back(id);
}

void map::move_layer_forward(const layer_id id)
{
  m_layers.move_elem_forward(id);
}

auto map::make_layer() -> std::shared_ptr<tile_layer>
{
  ++m_nextLayer;
  if (!m_activeLayer) {
    return std::make_shared<tile_layer>(5_row, 5_col);
  } else {
    return std::make_shared<tile_layer>(row_count(), col_count());
  }
}

auto map::tile_at(const position& position) const -> maybe<tile_id>
{
  return current_layer().tile_at(position);
}

auto map::index_of(const layer_id id) const -> maybe<int>
{
  return m_layers.index_of(id);
}

auto map::is_visible(const layer_id id) const -> bool
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

auto map::has_layer(const layer_id id) const -> bool
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

auto map::current_layer() -> tile_layer&
{
  return *m_layers.at(m_activeLayer.value());
}

auto map::current_layer() const -> const tile_layer&
{
  return *m_layers.at(m_activeLayer.value());
}

auto map::find_layer(const layer_id id) -> tile_layer*
{
  if (const auto it = m_layers.find(id); it != m_layers.end()) {
    return it->second.get();
  } else {
    return nullptr;
  }
}

auto map::find_layer(const layer_id id) const -> const tile_layer*
{
  if (const auto it = m_layers.find(id); it != m_layers.end()) {
    return it->second.get();
  } else {
    return nullptr;
  }
}

}  // namespace tactile::core
