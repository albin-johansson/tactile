#include "parse_tile_layer.hpp"

#include "core/map/layers/tile_layer.hpp"
#include "parse_tile_data.hpp"

namespace Tactile::IO {

auto ParseTileLayer(const nlohmann::json& json, LayerData& layer) -> ParseError
{
  auto& data = layer.data.emplace<TileLayerData>();

  if (const auto it = json.find("height"); it != json.end())
  {
    data.row_count = row_t{it->get<row_t::value_type>()};
  }
  else
  {
    return ParseError::LayerMissingHeight;
  }

  if (const auto it = json.find("width"); it != json.end())
  {
    data.col_count = col_t{it->get<col_t::value_type>()};
  }
  else
  {
    return ParseError::LayerMissingWidth;
  }

  data.tiles = MakeTileMatrix(data.row_count, data.col_count);
  if (const auto err = ParseTileData(json, data.col_count, data.tiles);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO