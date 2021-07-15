#include "save_properties.hpp"

#include <string>   // string
#include <utility>  // move

#include "core/tactile_error.hpp"
#include "io/saving/common.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveProperty(std::string name,
                                const Property& property,
                                const std::filesystem::path& dir) -> nlohmann::json
{
  auto json = nlohmann::json::object();
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
      value = property.AsInteger();
      break;
    }
    case PropertyType::Floating:
    {
      value = property.AsFloating();
      break;
    }
    case PropertyType::Boolean:
    {
      value = property.AsBoolean();
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

[[nodiscard]] auto SaveProperties(const IPropertyContext& context,
                                  const std::filesystem::path& dir) -> nlohmann::json
{
  auto array = nlohmann::json::array();

  for (const auto& [name, property] : context.GetProperties())
  {
    array += SaveProperty(name, property, dir);
  }

  return array;
}

}  // namespace Tactile::IO
