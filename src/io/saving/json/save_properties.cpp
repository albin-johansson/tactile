#include "save_properties.hpp"

#include <string>   // string
#include <utility>  // move

#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "core/tactile_error.hpp"
#include "io/saving/common.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveProperty(std::string name,
                                const PropertyValue& property,
                                const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();
  const auto type = property.GetType().value();

  json["name"] = std::move(name);
  json["type"] = GetPropertyTypeString(type);

  auto& value = json["value"];
  switch (type)
  {
    case PropertyType::String:
    {
      value = property.AsString();
      break;
    }
    case PropertyType::Integer:
    {
      value = property.AsInt();
      break;
    }
    case PropertyType::Floating:
    {
      value = property.AsFloat();
      break;
    }
    case PropertyType::Boolean:
    {
      value = property.AsBool();
      break;
    }
    case PropertyType::File:
    {
      value = GetPropertyFileValue(property, dir);
      break;
    }
    case PropertyType::Color:
    {
      value = property.AsColor().as_argb();
      break;
    }
    case PropertyType::Object:
    {
      value = property.AsObject().get();
      break;
    }
  }

  return json;
}

}  // namespace

[[nodiscard]] auto SaveProperties(const entt::registry& registry,
                                  const entt::entity entity,
                                  const std::filesystem::path& dir) -> JSON
{
  auto array = JSON::array();

  const auto& context = (entity != entt::null)
                            ? registry.get<PropertyContext>(entity)
                            : registry.ctx<PropertyContext>();
  for (const auto propertyEntity : context.properties)
  {
    const auto& property = registry.get<Property>(propertyEntity);
    array += SaveProperty(property.name, property.value, dir);
  }

  return array;
}

}  // namespace Tactile::IO
