#pragma once

#include <map>     // map
#include <string>  // string

#include "aliases/layer_id.hpp"
#include "aliases/not_null.hpp"
#include "aliases/tile_id.hpp"
#include "core/commands/command.hpp"
#include "core/map/map_position.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class MapDocument;
class Map;

class MapCommand : public ACommand
{
 public:
  using TileCache = std::map<MapPosition, tile_id>;
  using LayerCache = std::map<layer_id, TileCache>;

  MapCommand(NotNull<MapDocument*> document, std::string text);

 protected:
  void ClearCache() noexcept;

  void RestoreTiles();

  void SaveTiles(MapPosition begin, MapPosition end);

  [[nodiscard]] auto GetTileCache(layer_id id) -> TileCache&;

  [[nodiscard]] auto GetLayerCache() const -> const LayerCache&;

  [[nodiscard]] auto GetMap() -> Map&;

  [[nodiscard]] auto GetDocument() -> MapDocument*
  {
    return mDocument;
  }

  [[nodiscard]] auto GetDocument() const -> const MapDocument*
  {
    return mDocument;
  }

 private:
  MapDocument* mDocument{};
  LayerCache mCache;
};

/// \} End of group core

}  // namespace Tactile
