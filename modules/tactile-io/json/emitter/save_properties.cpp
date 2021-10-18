#include "save_properties.hpp"

#include <string>   // string
#include <utility>  // move

#include "common_saving.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveProperty(const PropertyData& property,
                                const std::filesystem::path& dir) -> JSON
{
  const auto& name = property.name;

  auto json = JSON::object();
  const auto type = property.property.GetType().value();

  json["name"] = name;
  json["type"] = GetPropertyTypeString(type);

  auto& value = json["value"];
  switch (type) {
    case PropertyType::String: {
      value = property.property.AsString();
      break;
    }
    case PropertyType::Integer: {
      value = property.property.AsInt();
      break;
    }
    case PropertyType::Floating: {
      value = property.property.AsFloat();
      break;
    }
    case PropertyType::Boolean: {
      value = property.property.AsBool();
      break;
    }
    case PropertyType::File: {
      value = GetPropertyFileValue(property.property, dir);
      break;
    }
    case PropertyType::Color: {
      value = property.property.AsColor().as_argb();
      break;
    }
    case PropertyType::Object: {
      value = property.property.AsObject().get();
      break;
    }
  }

  return json;
}

}  // namespace

[[nodiscard]] auto SaveProperties(const std::vector<PropertyData>& properties,
                                  const std::filesystem::path& dir) -> JSON
{
  auto array = JSON::array();

  for (const auto& property : properties) {
    array += SaveProperty(property, dir);
  }

  return array;
}

}  // namespace Tactile::IO
