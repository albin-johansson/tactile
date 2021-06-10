#include "map_command.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

MapCommand::MapCommand(not_null<core::MapDocument*> document,
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
  auto& map = mDocument->Raw();

  const auto activeLayer = map.ActiveLayerId().value();

  for (const auto& [layer, data] : mLayerData)
  {
    map.SelectLayer(layer);

    auto* tileLayer = map.GetTileLayer(layer);
    Q_ASSERT(tileLayer);

    for (const auto& [pos, tile] : data)
    {
      tileLayer->SetTile(pos, tile);
    }
  }

  map.SelectLayer(activeLayer);
}

void MapCommand::SaveTiles(const row_range rows, const col_range cols)
{
  auto& map = mDocument->Raw();

  map.EachLayer([&](const layer_id id, const shared<core::ILayer>& layer) {
    auto& tiles = TileData(id);

    auto* tileLayer = map.GetTileLayer(id);
    Q_ASSERT(tileLayer);

    for (auto row = rows.first; row < rows.second; ++row)
    {
      for (auto col = cols.first; col < cols.second; ++col)
      {
        const core::Position pos{row, col};
        if (const auto tile = tileLayer->TileAt(pos); tile)
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
  emit mDocument->S_Redraw();
}

auto MapCommand::GetMap() noexcept -> core::Map&
{
  return mDocument->Raw();
}

}  // namespace tactile::cmd
