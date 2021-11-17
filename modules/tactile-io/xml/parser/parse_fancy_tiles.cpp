#include "parse_fancy_tiles.hpp"

#include "parse_object.hpp"
#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseFancyTiles(const pugi::xml_node node, Tileset& tileset) -> ParseError
{
  for (const auto tileNode : node.children("tile")) {
    auto& tile = AddTile(tileset);
    SetId(tile, GetInt(tileNode, "id").value());

    for (const auto frameNode : tileNode.child("animation").children("frame")) {
      auto& frame = AddAnimationFrame(tile);
      SetTile(frame, GetInt(frameNode, "tileid").value());
      SetDuration(frame, GetInt(frameNode, "duration").value());
    }

    for (const auto objectNode : tileNode.child("objectgroup").children("object")) {
      auto& object = AddObject(tile);
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
