#include "parse_tile_layer.hpp"

#include <algorithm>  // replace
#include <string>     // string

#include <tactile_stdlib.hpp>

#include <yaml-cpp/yaml.h>

#include "string_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseTiles(TileLayer& layer,
                              const std::string& data,
                              const usize nCols) -> ParseError
{
  usize index = 0;
  for (const auto& token : Split(data.c_str(), ' ')) {
    if (const auto id = FromString<int32>(token.c_str())) {
      const auto [row, col] = ToMatrixCoords(index, nCols);
      SetTile(layer, row, col, *id);

      ++index;
    }
    else {
      return ParseError::CouldNotParseTiles;
    }
  }

  return ParseError::None;
}

}  // namespace

auto ParseTileLayer(const YAML::Node& node,
                    Layer& layer,
                    const usize nRows,
                    const usize nCols) -> ParseError
{
  auto& tileLayer = MarkAsTileLayer(layer);
  ReserveTiles(tileLayer, nRows, nCols);

  if (auto tiles = node["data"]) {
    auto raw = tiles.as<std::string>();
    std::replace(raw.begin(), raw.end(), '\n', ' ');

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
