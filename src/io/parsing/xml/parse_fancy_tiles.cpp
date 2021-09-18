#include "parse_fancy_tiles.hpp"

#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseFancyTiles(const pugi::xml_node node, TilesetData& data) -> ParseError
{
  for (const auto tile : node.children("tile")) {
    auto& tileData = data.tiles.emplace_back();
    tileData.id = TileID{GetInt(tile, "id").value()};

    for (const auto frame : tile.child("animation").children("frame")) {
      auto& frameData = tileData.animation.emplace_back();
      frameData.tile = TileID{GetInt(frame, "tileid").value()};
      frameData.duration = GetInt(frame, "duration").value();
    }

    if (const auto err = ParseProperties(tile, tileData.properties);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
