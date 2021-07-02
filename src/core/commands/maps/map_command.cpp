#include "map_command.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/map_document.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

MapCommand::MapCommand(NotNull<MapDocument*> document, std::string text)
    : ACommand{std::move(text)}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create map command from null document!"};
  }
}

void MapCommand::ClearCache() noexcept
{
  mCache.clear();
}

void MapCommand::RestoreTiles()
{
  auto& map = GetMap();
  const auto activeLayer = map.GetActiveLayerId().value();

  for (const auto& [layerId, tileCache] : mCache)
  {
    map.SelectLayer(layerId);

    auto* tileLayer = map.GetTileLayer(layerId);
    assert(tileLayer);

    for (const auto& [position, tileId] : tileCache)
    {
      tileLayer->SetTile(position, tileId);
    }
  }

  map.SelectLayer(activeLayer);
}

void MapCommand::SaveTiles(const MapPosition begin, const MapPosition end)
{
  auto& map = GetMap();

  for (const auto& [id, layer] : map)
  {
    if (auto* tileLayer = map.GetTileLayer(id))
    {
      auto& tileCache = GetTileCache(id);

      const auto endRow = end.GetRow();
      const auto endCol = end.GetColumn();
      for (auto row = begin.GetRow(); row < endRow; ++row)
      {
        for (auto col = begin.GetColumn(); col < endCol; ++col)
        {
          const MapPosition position{row, col};
          if (const auto tile = tileLayer->GetTile(position))
          {
            tileCache.emplace(position, *tile);
          }
        }
      }
    }
  }
}

auto MapCommand::GetTileCache(const layer_id id) -> TileCache&
{
  // Default constructs new map if layer wasn't present
  return mCache[id];
}

auto MapCommand::GetMap() -> Map&
{
  return mDocument->GetMap();
}

}  // namespace Tactile
