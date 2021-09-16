#include "parse_tile_layer.hpp"

#include "core/systems/layers/tile_layer_system.hpp"
#include "parse_tile_data.hpp"

namespace Tactile::IO {

auto ParseTileLayer(const JSON& json, LayerData& layer) -> ParseError
{
  auto& data = layer.data.emplace<TileLayerData>();

  if (const auto it = json.find("height"); it != json.end())
  {
    it->get_to(data.row_count);
  }
  else
  {
    return ParseError::LayerMissingHeight;
  }

  if (const auto it = json.find("width"); it != json.end())
  {
    it->get_to(data.col_count);
  }
  else
  {
    return ParseError::LayerMissingWidth;
  }

  data.tiles = Sys::MakeTileMatrix(data.row_count, data.col_count);
  if (const auto err = ParseTileData(json, data.col_count, data.tiles);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO