#include "remove_row_col.hpp"

#include "tactile_error.hpp"

namespace tactile::cmd {

remove_row_col::remove_row_col(core::map* map, const QString& name)
    : QUndoCommand{name},
      m_map{map}
{
  if (!m_map) {
    throw tactile_error{"Cannot create command from null map pointer!"};
  }
}

void remove_row_col::restore_tiles()
{
  const auto activeLayer = m_map->active_layer_id();

  for (const auto& [layer, data] : m_layerData) {
    m_map->select_layer(layer);
    for (const auto& [pos, tile] : data) {
      m_map->set_tile(pos, tile);
    }
  }

  m_map->select_layer(activeLayer);
}

void remove_row_col::save_tiles(row_range rows, col_range cols)
{
  auto* map = get_map();
  map->each_layer([&](layer_id id, const core::layer& layer) {
    auto& data = layer_data(id);
    for (auto row = rows.first; row < rows.second; ++row) {
      for (auto col = cols.first; col < cols.second; ++col) {
        const core::position pos{row, col};
        if (const auto tile = layer.tile_at(pos); tile) {
          data.emplace(pos, *tile);
        }
      }
    }
  });
}

auto remove_row_col::mergeWith(const QUndoCommand* other) -> bool
{
  if (id() == other->id()) {
    if (auto* ptr = dynamic_cast<const remove_row_col*>(other)) {
      m_times += ptr->m_times;

      for (const auto& [layer, data] : ptr->m_layerData) {
        if (const auto it = m_layerData.find(layer); it != m_layerData.end()) {
          for (const auto& [pos, tile] : data) {
            it->second.emplace(pos, tile);
          }
        } else {
          m_layerData.emplace(layer, data);
        }
      }

      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd
