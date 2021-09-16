#include "parse_tile_layer.hpp"

#include "core/systems/layers/tile_layer_system.hpp"
#include "parse_tile_data.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseTileLayer(const pugi::xml_node node, LayerData& layer) -> ParseError
{
  auto& data = layer.data.emplace<TileLayerData>();

  if (const auto width = GetInt(node, "width")) {
    data.col_count = *width;
  }
  else {
    return ParseError::LayerMissingWidth;
  }

  if (const auto height = GetInt(node, "height")) {
    data.row_count = *height;
  }
  else {
    return ParseError::LayerMissingHeight;
  }

  data.tiles = Sys::MakeTileMatrix(data.row_count, data.col_count);
  if (const auto err = ParseTileData(node, data.col_count, data.tiles);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
