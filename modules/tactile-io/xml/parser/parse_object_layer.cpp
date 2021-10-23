#include "parse_object_layer.hpp"

#include "parse_object.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseObjectLayer(const pugi::xml_node node, Layer& layer) -> ParseError
{
  auto& objectLayer = IO::MarkAsObjectLayer(layer);

  for (const auto objectNode : node.children("object")) {
    auto& object = IO::AddObject(objectLayer);
    if (const auto err = ParseObject(objectNode, object); err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
