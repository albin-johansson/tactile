#include "parse_tile_layer.hpp"

#include "parse_tile_data.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseTileLayer(const pugi::xml_node node, Layer& layer) -> ParseError
{
  auto& tileLayer = MarkAsTileLayer(layer);

  const auto width = GetInt(node, "width");
  const auto height = GetInt(node, "height");

  if (!width) {
    return ParseError::LayerMissingWidth;
  }

  if (!height) {
    return ParseError::LayerMissingHeight;
  }

  ReserveTiles(tileLayer, *height, *width);
  if (const auto err = ParseTileData(node, tileLayer); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
