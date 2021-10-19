#include "parse_tile_layer.hpp"

#include <algorithm>  // replace
#include <string>     // string
#include <utility>    // move

#include <tactile-base/from_string.hpp>
#include <tactile-base/map_position.hpp>
#include <tactile-base/string_utils.hpp>

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseTiles(const std::string& data, const int nRows, const int nCols)
    -> tl::expected<TileMatrix, ParseError>
{
  auto matrix = MakeTileMatrix(nRows, nCols);

  int index = 0;
  for (const auto& token : Split(data.c_str(), ' ')) {
    if (const auto id = FromString<TileID::value_type>(token)) {
      const auto pos = MapPosition::FromIndex(index, nCols);
      matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = TileID{*id};
    }
    else {
      return tl::make_unexpected(ParseError::CouldNotParseTiles);
    }

    ++index;
  }

  return matrix;
}

}  // namespace

auto ParseTileLayer(const YAML::Node& node, const int nRows, const int nCols)
    -> tl::expected<TileLayerData, ParseError>
{
  TileLayerData data;
  data.row_count = nRows;
  data.col_count = nCols;

  if (auto tiles = node["data"]) {
    auto raw = tiles.as<std::string>();
    std::ranges::replace(raw, '\n', ' ');

    if (auto matrix = ParseTiles(raw, nRows, nCols)) {
      data.tiles = std::move(*matrix);
    }
    else {
      return tl::make_unexpected(matrix.error());
    }
  }
  else {
    return tl::make_unexpected(ParseError::CouldNotParseTiles);
  }

  return data;
}

}  // namespace Tactile::IO
