#include "map.hpp"

#include <QDebug>  // Q_ASSERT

#include "algorithm.hpp"
#include "layer_utils.hpp"
#include "object_layer.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::core {

map::map(const row_t nRows, const col_t nCols) : m_rows{nRows}, m_cols{nCols}
{
  if (m_rows < 1_row || m_cols < 1_col)
  {
    throw tactile_error{"Invalid map dimensions!"};
  }

  m_layers.reserve(5);
  m_layers.emplace(m_nextLayer, std::make_shared<tile_layer>(m_rows, m_cols));

  m_activeLayer = m_nextLayer;
  ++m_nextLayer;
}

void map::remove_occurrences(const tile_id id)
{
  for (auto& [key, layer] : m_layers)
  {
    if (auto* tileLayer = as_tile_layer(layer))
    {
      tileLayer->remove_all(id);
    }
  }
}

void map::remove_layer(const layer_id id)
{
  Q_ASSERT(m_layers.contains(id));

  if (m_activeLayer == id)
  {
    m_activeLayer.reset();
  }

  m_layers.erase(id);
}

auto map::take_layer(const layer_id id) -> shared<layer>
{
  Q_ASSERT(m_layers.contains(id));

  if (m_activeLayer == id)
  {
    m_activeLayer.reset();
  }

  auto layer = m_layers.at(id);
  m_layers.erase(id);

  return layer;
}

void map::select_layer(const layer_id id)
{
  if (m_layers.contains(id))
  {
    m_activeLayer = id;
  }
}

auto map::add_tile_layer() -> layer_id
{
  const auto id = m_nextLayer;
  add_layer(id, make_tile_layer());
  return id;
}

auto map::add_object_layer() -> layer_id
{
  const auto id = m_nextLayer;
  add_layer(id, make_object_layer());
  return id;
}

void map::add_layer(const layer_id id, shared<layer> layer)
{
  // TODO what happens if dimensions mismatch?

  Q_ASSERT(!m_layers.contains(id));
  Q_ASSERT(layer);

  if (m_layers.empty())
  {
    if (const auto* tileLayer = as_tile_layer(layer))
    {
      m_rows = tileLayer->row_count();
      m_cols = tileLayer->col_count();
    }
  }

  m_layers.emplace(id, std::move(layer));
}

auto map::duplicate_layer(const layer_id id) -> layer_pair&
{
  Q_ASSERT(m_layers.contains(id));
  const auto& layer = m_layers.at(id);

  const auto newId = m_nextLayer;
  auto copy = layer->clone();

  auto& pair = m_layers.emplace(newId, std::move(copy));

  ++m_nextLayer;
  return pair;
}

void map::add_row(const tile_id id)
{
  for (auto& [key, layer] : m_layers)
  {
    if (auto* tileLayer = as_tile_layer(layer))
    {
      tileLayer->add_row(id);
    }
  }
  ++m_rows;
}

void map::add_col(const tile_id id)
{
  for (auto& [key, layer] : m_layers)
  {
    if (auto* tileLayer = as_tile_layer(layer))
    {
      tileLayer->add_col(id);
    }
  }
  ++m_cols;
}

void map::remove_row()
{
  if (row_count() == 1_row)
  {
    return;
  }

  for (auto& [key, layer] : m_layers)
  {
    if (auto* tileLayer = as_tile_layer(layer))
    {
      tileLayer->remove_row();
    }
  }

  --m_rows;
}

void map::remove_col()
{
  if (col_count() == 1_col)
  {
    return;
  }

  for (auto& [key, layer] : m_layers)
  {
    if (auto* tileLayer = as_tile_layer(layer))
    {
      tileLayer->remove_col();
    }
  }

  --m_cols;
}

void map::increase_tile_size()
{
  m_tileSize.increase();
}

void map::decrease_tile_size() noexcept
{
  m_tileSize.decrease();
}

void map::reset_tile_size() noexcept
{
  m_tileSize.reset();
}

void map::set_next_layer_id(const layer_id id) noexcept
{
  Q_ASSERT(!has_layer(id));
  m_nextLayer = id;
}

void map::set_next_object_id(const object_id id) noexcept
{
  m_nextObject = id;
}

void map::set_row_count(row_t nRows)
{
  nRows = at_least(nRows, 1_row);

  if (nRows == row_count())
  {
    return;
  }

  m_rows = nRows;
  for (auto& [key, layer] : m_layers)
  {
    if (auto* tileLayer = as_tile_layer(layer))
    {
      tileLayer->set_rows(m_rows);
    }
  }
}

void map::set_col_count(col_t nCols)
{
  nCols = at_least(nCols, 1_col);

  if (nCols == col_count())
  {
    return;
  }

  m_cols = nCols;
  for (auto& [key, layer] : m_layers)
  {
    if (auto* tileLayer = as_tile_layer(layer))
    {
      tileLayer->set_cols(m_cols);
    }
  }
}

void map::set_visibility(const layer_id id, const bool visible)
{
  if (auto* layer = find_layer(id))
  {
    layer->set_visible(visible);
  }
}

void map::set_opacity(const layer_id id, const double opacity)
{
  if (auto* layer = find_layer(id))
  {
    layer->set_opacity(opacity);
  }
}

void map::set_name(const layer_id id, const QString& name)
{
  if (auto* layer = find_layer(id))
  {
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

auto map::make_tile_layer() -> shared<tile_layer>
{
  ++m_nextLayer;
  if (!m_activeLayer)
  {
    return std::make_shared<tile_layer>(5_row, 5_col);
  }
  else
  {
    return std::make_shared<tile_layer>(row_count(), col_count());
  }
}

auto map::make_object_layer() -> shared<object_layer>
{
  ++m_nextLayer;
  return std::make_shared<object_layer>();
}

auto map::index_of(const layer_id id) const -> maybe<int>
{
  return m_layers.index_of(id);
}

auto map::is_visible(const layer_id id) const -> bool
{
  if (const auto* layer = find_layer(id))
  {
    return layer->visible();
  }
  else
  {
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
  const auto endRow = m_rows;
  const auto endCol = m_cols;

  const auto [row, col] = pos.unpack();
  return (row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol);
}

auto map::row_count() const -> row_t
{
  return m_rows;
}

auto map::col_count() const -> col_t
{
  return m_cols;
}

auto map::width() const -> int
{
  return m_cols.get() * m_tileSize.get();
}

auto map::height() const -> int
{
  return m_rows.get() * m_tileSize.get();
}

auto map::active_layer_id() const noexcept -> maybe<layer_id>
{
  return m_activeLayer;
}

auto map::next_layer_id() const noexcept -> layer_id
{
  return m_nextLayer;
}

auto map::current_tile_size() const noexcept -> int
{
  return m_tileSize.get();
}

auto map::get_layer(const layer_id id) const -> const shared<layer>&
{
  return m_layers.at(id);
}

auto map::get_layer(const layer_id id) -> shared<layer>&
{
  return m_layers.at(id);
}

auto map::get_tile_layer(const layer_id id) -> tile_layer*
{
  return as_tile_layer(get_layer(id));
}

auto map::get_tile_layer(const layer_id id) const -> const tile_layer*
{
  return as_tile_layer(get_layer(id));
}

auto map::get_object_layer(layer_id id) -> object_layer*
{
  return as_object_layer(get_layer(id));
}

auto map::get_object_layer(layer_id id) const -> const object_layer*
{
  return as_object_layer(get_layer(id));
}

auto map::find_layer(const layer_id id) -> layer*
{
  if (const auto it = m_layers.find(id); it != m_layers.end())
  {
    return it->second.get();
  }
  else
  {
    return nullptr;
  }
}

auto map::find_layer(const layer_id id) const -> const layer*
{
  if (const auto it = m_layers.find(id); it != m_layers.end())
  {
    return it->second.get();
  }
  else
  {
    return nullptr;
  }
}

}  // namespace tactile::core
