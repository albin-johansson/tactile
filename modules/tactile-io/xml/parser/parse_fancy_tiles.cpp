#include "parse_fancy_tiles.hpp"

#include "parse_object.hpp"
#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseFancyTiles(const pugi::xml_node node, Tileset& tileset) -> ParseError
{
  for (const auto tileNode : node.children("tile")) {
    auto& tile = IO::AddTile(tileset);
    IO::SetId(tile, GetInt(tileNode, "id").value());

    for (const auto frameNode : tileNode.child("animation").children("frame")) {
      auto& frame = IO::AddAnimationFrame(tile);
      IO::SetTile(frame, GetInt(frameNode, "tileid").value());
      IO::SetDuration(frame, GetInt(frameNode, "duration").value());
    }

    for (const auto objectNode : tileNode.child("objectgroup").children("object")) {
      auto& object = IO::AddObject(tile);
      if (const auto err = ParseObject(objectNode, object); err != ParseError::None) {
        return err;
      }
    }

    if (const auto err = ParseProperties(tileNode, tile); err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
