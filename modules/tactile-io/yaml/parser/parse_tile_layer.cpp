#include "parse_tile_layer.hpp"

#include <algorithm>  // replace
#include <string>     // string

#include <tactile-base/from_string.hpp>
#include <tactile-base/map_position.hpp>
#include <tactile-base/string_utils.hpp>
#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseTiles(TileLayer& layer,
                              const std::string& data,
                              const int32 nCols) -> ParseError
{
  int32 index = 0;
  for (const auto& token : Split(data.c_str(), ' ')) {
    if (const auto id = FromString<TileID::value_type>(token)) {
      const auto pos = MapPosition::FromIndex(index, nCols);
      SetTile(layer, pos.GetRow(), pos.GetColumn(), *id);
    }
    else {
      return ParseError::CouldNotParseTiles;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace

auto ParseTileLayer(const YAML::Node& node,
                    Layer& layer,
                    const int32 nRows,
                    const int32 nCols) -> ParseError
{
  auto& tileLayer = MarkAsTileLayer(layer);
  ReserveTiles(tileLayer, nRows, nCols);

  if (auto tiles = node["data"]) {
    auto raw = tiles.as<std::string>();
    std::ranges::replace(raw, '\n', ' ');

    if (const auto err = ParseTiles(tileLayer, raw, nCols); err != ParseError::None) {
      return err;
    }
  }
  else {
    return ParseError::CouldNotParseTiles;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
