#include "parse_tile_layer.hpp"

#include <string>   // string
#include <utility>  // move

#include "core/map_position.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "utils/from_string.hpp"
#include "utils/string_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseTiles(const std::string& data, const int nRows, const int nCols)
    -> Expected<TileMatrix, ParseError>
{
  auto matrix = Sys::MakeTileMatrix(nRows, nCols);

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
    -> Expected<TileLayerData, ParseError>
{
  TileLayerData data;
  data.row_count = nRows;
  data.col_count = nCols;

  if (auto tiles = node["data"]) {
    if (auto matrix = ParseTiles(tiles.as<std::string>(), nRows, nCols)) {
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
