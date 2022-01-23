#include "parse_tile_layer.hpp"

#include "parse_tile_data.hpp"

namespace tactile::IO {

auto ParseTileLayer(const JSON& json, Layer& layer) -> ParseError
{
  auto& tileLayer = MarkAsTileLayer(layer);

  int32 nRows{};
  int32 nCols{};

  if (const auto it = json.find("height"); it != json.end()) {
    nRows = it->get<int32>();
  }
  else {
    return ParseError::LayerMissingHeight;
  }

  if (const auto it = json.find("width"); it != json.end()) {
    nCols = it->get<int32>();
  }
  else {
    return ParseError::LayerMissingWidth;
  }

  ReserveTiles(tileLayer, nRows, nCols);
  if (const auto err = ParseTileData(json, tileLayer); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace tactile::IO