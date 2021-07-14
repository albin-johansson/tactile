#include "save_properties.hpp"

#include <string>   // string
#include <utility>  // move

#include "core/tactile_error.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto Stringify(const PropertyType type) -> std::string
{
  switch (type)
  {
    case PropertyType::String:
      return "string";

    case PropertyType::Integer:
      return "int";

    case PropertyType::Floating:
      return "float";

    case PropertyType::Boolean:
      return "bool";

    case PropertyType::File:
      return "file";

    case PropertyType::Color:
      return "color";

    case PropertyType::Object:
      return "object";

    default:
      throw TactileError{"Did not recognize property type!"};
  }
}

[[nodiscard]] auto SaveProperty(std::string name,
                                const Property& property,
                                const std::filesystem::path& dir) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["name"] = std::move(name);
  json["type"] = Stringify(property.GetType().value());

  auto& value = json["value"];
  switch (property.GetType().value())
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
      const auto path = std::filesystem::proximate(property.AsFile(), dir);
      value = path.string();
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
