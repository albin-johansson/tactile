#include "parse_object_layer.hpp"

#include <string>   // string
#include <utility>  // move

#include <yaml-cpp/yaml.h>

#include "parse_object.hpp"
#include "parse_properties.hpp"

namespace Tactile::IO {

auto ParseObjectLayer(const YAML::Node& node) -> tl::expected<ObjectLayerData, ParseError>
{
  ObjectLayerData data;

  if (auto objects = node["objects"]) {
    data.objects.reserve(objects.size());

    for (const auto& objectNode : objects) {
      if (auto object = ParseObject(objectNode)) {
        data.objects.push_back(std::move(*object));
      }
      else {
        return tl::make_unexpected(object.error());
      }
    }
  }

  return data;
}

}  // namespace Tactile::IO