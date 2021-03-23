#include "map_command.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

map_command::map_command(not_null<core::map_document*> document,
                         const QString& name)
    : QUndoCommand{name}
    , m_document{document}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void map_command::restore_tiles()
{
  auto& map = m_document->raw();

  const auto activeLayer = map.active_layer_id().value();

  for (const auto& [layer, data] : m_layerData)
  {
    map.select_layer(layer);

    auto* tileLayer = map.get_tile_layer(layer);
    Q_ASSERT(tileLayer);

    for (const auto& [pos, tile] : data)
    {
      tileLayer->set_tile(pos, tile);
    }
  }

  map.select_layer(activeLayer);
}

void map_command::save_tiles(row_range rows, col_range cols)
{
  auto& map = m_document->raw();

  map.each_layer([&](const layer_id id, const shared<core::layer>& layer) {
    auto& tiles = tile_data(id);

    auto* tileLayer = map.get_tile_layer(id);
    Q_ASSERT(tileLayer);

    for (auto row = rows.first; row < rows.second; ++row)
    {
      for (auto col = cols.first; col < cols.second; ++col)
      {
        const core::position pos{row, col};
        if (const auto tile = tileLayer->tile_at(pos); tile)
        {
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

void map_command::redraw()
{
  emit m_document->redraw();
}

auto map_command::get_map() noexcept -> core::map&
{
  return m_document->raw();
}

}  // namespace tactile::cmd
