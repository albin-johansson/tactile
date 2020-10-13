#include "map_command.hpp"

#include "tactile_error.hpp"

namespace tactile::cmd {

map_command::map_command(core::map* map, const QString& name)
    : QUndoCommand{name},
      m_map{map}
{
  if (!m_map) {
    throw tactile_error{"Cannot create command from null map pointer!"};
  }
}

void map_command::restore_tiles()
{
  const auto activeLayer = m_map->active_layer_id().value();

  for (const auto& [layer, data] : m_layerData) {
    m_map->select_layer(layer);
    for (const auto& [pos, tile] : data) {
      m_map->set_tile(pos, tile);
    }
  }

  m_map->select_layer(activeLayer);
}

void map_command::save_tiles(map_command::row_range rows,
                             map_command::col_range cols)
{
  m_map->each_layer([&](layer_id id, const core::layer& layer) {
    auto& tiles = tile_data(id);
    for (auto row = rows.first; row < rows.second; ++row) {
      for (auto col = cols.first; col < cols.second; ++col) {
        const core::position pos{row, col};
        if (const auto tile = layer.tile_at(pos); tile) {
          tiles.emplace(pos, *tile);
        }
      }
    }
  });
}

void map_command::clear_cache()
{
  m_layerData.clear();
}

}  // namespace tactile::cmd
