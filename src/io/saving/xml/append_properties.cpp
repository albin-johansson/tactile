#include "append_properties.hpp"

#include "core/tactile_error.hpp"
#include "io/saving/common.hpp"

namespace Tactile::IO {
namespace {

void AppendTypeAttribute(const PropertyType type, pugi::xml_node& node)
{
  if (type != PropertyType::String)
  {
    const auto str = GetPropertyTypeString(type);
    node.append_attribute("type").set_value(str.c_str());
  }
}

void AppendValueAttribute(const Property& property,
                          pugi::xml_node& node,
                          const std::filesystem::path& dir)
{
  auto valueAttr = node.append_attribute("value");
  switch (property.GetType().value())
  {
    case PropertyType::String:
      valueAttr.set_value(property.AsString().c_str());
      break;

    case PropertyType::Integer:
      valueAttr.set_value(property.AsInt());
      break;

    case PropertyType::Floating:
      valueAttr.set_value(property.AsFloat());
      break;

    case PropertyType::Boolean:
      valueAttr.set_value(property.AsBool());
      break;

    case PropertyType::File:
      valueAttr.set_value(GetPropertyFileValue(property, dir).c_str());
      break;

    case PropertyType::Color:
    {
      const auto argb = property.AsColor().as_argb();
      valueAttr.set_value(argb.c_str());
      break;
    }
    case PropertyType::Object:
      valueAttr.set_value(property.AsObject().get());
      break;

    default:
      throw TactileError{"Could not recognize property type!"};
  }
}

}  // namespace

void AppendProperties(const IPropertyContext& context,
                      pugi::xml_node& node,
                      const std::filesystem::path& dir)
{
  if (context.GetPropertyCount() > 0)
  {
    auto root = node.append_child("properties");
    for (const auto& [name, property] : context.GetProperties())
    {
      auto propertyNode = root.append_child("property");
      propertyNode.append_attribute("name").set_value(name.c_str());
      AppendTypeAttribute(property.GetType().value(), propertyNode);
      AppendValueAttribute(property, propertyNode, dir);
    }
  }
}

}  // namespace Tactile::IO
