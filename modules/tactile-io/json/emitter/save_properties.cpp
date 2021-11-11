#include "save_properties.hpp"

#include <string>   // string
#include <utility>  // move

#include "common_saving.hpp"
#include "ir_helpers.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveProperty(const Property& property,
                                const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  const auto* name = GetName(property);
  const auto type = GetType(property);

  json["name"] = name;
  json["type"] = GetPropertyTypeString(type);

  auto& value = json["value"];
  switch (type) {
    case PropertyType::String: {
      value = GetString(property);
      break;
    }
    case PropertyType::Integer: {
      value = GetInt(property);
      break;
    }
    case PropertyType::Floating: {
      value = GetFloat(property);
      break;
    }
    case PropertyType::Boolean: {
      value = GetBool(property);
      break;
    }
    case PropertyType::File: {
      value = GetPropertyFileValue(property, dir);
      break;
    }
    case PropertyType::Color: {
      value = AsARGB(GetColor(property));
      break;
    }
    case PropertyType::Object: {
      value = GetObject(property);
      break;
    }
  }

  return json;
}

template <typename T>
[[nodiscard]] auto SavePropertiesImpl(const T& source, const std::filesystem::path& dir)
    -> JSON
{
  auto array = JSON::array();

  EachProperty(source,
               [&](const Property& property) { array += SaveProperty(property, dir); });

  return array;
}

}  // namespace

auto SaveProperties(const Map& map, const std::filesystem::path& dir) -> JSON
{
  return SavePropertiesImpl(map, dir);
}

auto SaveProperties(const Layer& layer, const std::filesystem::path& dir) -> JSON
{
  return SavePropertiesImpl(layer, dir);
}

auto SaveProperties(const Tileset& tileset, const std::filesystem::path& dir) -> JSON
{
  return SavePropertiesImpl(tileset, dir);
}

auto SaveProperties(const Tile& tile, const std::filesystem::path& dir) -> JSON
{
  return SavePropertiesImpl(tile, dir);
}

auto SaveProperties(const Object& object, const std::filesystem::path& dir) -> JSON
{
  return SavePropertiesImpl(object, dir);
}

}  // namespace Tactile::IO
