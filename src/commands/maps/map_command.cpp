#include "map_command.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

MapCommand::MapCommand(not_null<core::map_document*> document,
                       const QString& name)
    : QUndoCommand{name}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void MapCommand::RestoreTiles()
{
  auto& map = mDocument->raw();

  const auto activeLayer = map.active_layer_id().value();

  for (const auto& [layer, data] : mLayerData)
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

void MapCommand::SaveTiles(const row_range rows, const col_range cols)
{
  auto& map = mDocument->raw();

  map.each_layer([&](const layer_id id, const shared<core::layer>& layer) {
    auto& tiles = TileData(id);

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

void MapCommand::ClearCache()
{
  mLayerData.clear();
}

void MapCommand::Redraw()
{
  emit mDocument->redraw();
}

auto MapCommand::GetMap() noexcept -> core::map&
{
  return mDocument->raw();
}

}  // namespace tactile::cmd
