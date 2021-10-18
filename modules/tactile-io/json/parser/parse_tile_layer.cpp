#include "parse_tile_layer.hpp"

#include <utility>  // move

#include <json.hpp>  // json

#include "parse_tile_data.hpp"

namespace Tactile::IO {

auto ParseTileLayer(const JSON& json) -> tl::expected<TileLayerData, ParseError>
{
  TileLayerData data;

  if (const auto it = json.find("height"); it != json.end()) {
    it->get_to(data.row_count);
  }
  else {
    return tl::make_unexpected(ParseError::LayerMissingHeight);
  }

  if (const auto it = json.find("width"); it != json.end()) {
    it->get_to(data.col_count);
  }
  else {
    return tl::make_unexpected(ParseError::LayerMissingWidth);
  }

  if (auto tiles = ParseTileData(json, data.row_count, data.col_count)) {
    data.tiles = std::move(*tiles);
  }
  else {
    return tl::make_unexpected(tiles.error());
  }

  return data;
}

}  // namespace Tactile::IO