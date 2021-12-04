#include "parse_object_layer.hpp"

#include <yaml-cpp/yaml.h>

#include "parse_object.hpp"

namespace Tactile::IO {

auto ParseObjectLayer(const YAML::Node& node, Layer& layer) -> ParseError
{
  auto& objectLayer = MarkAsObjectLayer(layer);

  if (auto seq = node["objects"]) {
    ReserveObjects(objectLayer, seq.size());

    for (const auto& objectNode : seq) {
      if (const auto err = ParseObject(objectNode, objectLayer);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO