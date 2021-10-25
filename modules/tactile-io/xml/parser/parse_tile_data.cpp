#include "parse_tile_data.hpp"

#include <cstring>  // strcmp

#include <tactile-base/tactile_std.hpp>

#include "parse_csv.hpp"
#include "parse_tile_nodes.hpp"

namespace Tactile::IO {

auto ParseTileData(const pugi::xml_node node, TileLayer& layer) -> ParseError
{
  const auto data = node.child("data");

  /* The encoding attribute is optional, if it is missing then the tile data is
     stored as individual "tile" nodes. */
  if (const auto* encoding = data.attribute("encoding").as_string(nullptr)) {
    // We only support the CSV encoding (when explicit)
    if (std::strcmp(encoding, "csv") != 0) {
      return ParseError::UnsupportedTileEncoding;
    }
    else {
      const auto text = data.text();
      if (const auto error = ParseCSV(text.get(), layer); error != ParseError::None) {
        return error;
      }
    }
  }
  else {
    if (const auto error = ParseTileNodes(data, layer); error != ParseError::None) {
      return error;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
