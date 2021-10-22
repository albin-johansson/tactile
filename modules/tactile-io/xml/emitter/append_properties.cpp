#include "append_properties.hpp"

#include <tactile-base/tactile_error.hpp>

#include "../../common_saving.hpp"

namespace Tactile::IO {
namespace {

void AppendProperty(pugi::xml_node root,
                    const Property& property,
                    const std::filesystem::path& dir)
{
  auto node = root.append_child("property");
  node.append_attribute("name").set_value(GetName(property));

  const auto type = GetType(property);

  if (type != PropertyType::String) {
    const auto str = GetPropertyTypeString(type);
    node.append_attribute("type").set_value(str.c_str());
  }

  auto valueAttr = node.append_attribute("value");
  switch (type) {
    case PropertyType::String:
      valueAttr.set_value(GetString(property));
      break;

    case PropertyType::Integer:
      valueAttr.set_value(GetInt(property));
      break;

    case PropertyType::Floating:
      valueAttr.set_value(GetFloat(property));
      break;

    case PropertyType::Boolean:
      valueAttr.set_value(GetBool(property));
      break;

    case PropertyType::File:
      valueAttr.set_value(GetPropertyFileValue(property, dir).c_str());
      break;

    case PropertyType::Color: {
      const auto color = GetColor(property);
      const auto argb =
          cen::color{color.red, color.green, color.blue, color.alpha}.as_argb();
      valueAttr.set_value(argb.c_str());
      break;
    }
    case PropertyType::Object:
      valueAttr.set_value(GetObject(property));
      break;

    default:
      throw TactileError{"Could not recognize property type!"};
  }
}

template <typename T>
void AppendPropertiesImpl(pugi::xml_node node,
                          const T& source,
                          const std::filesystem::path& dir)
{
  const auto count = GetPropertyCount(source);
  if (count != 0) {
    auto root = node.append_child("properties");
    for (usize index = 0; index < count; ++index) {
      const auto& property = GetProperty(source, index);
      AppendProperty(root, property, dir);
    }
  }
}

}  // namespace

void AppendProperties(pugi::xml_node node,
                      const Map& map,
                      const std::filesystem::path& dir)
{
  AppendPropertiesImpl(node, map, dir);
}

void AppendProperties(pugi::xml_node node,
                      const Layer& layer,
                      const std::filesystem::path& dir)
{
  AppendPropertiesImpl(node, layer, dir);
}

void AppendProperties(pugi::xml_node node,
                      const Tileset& tileset,
                      const std::filesystem::path& dir)
{
  AppendPropertiesImpl(node, tileset, dir);
}

void AppendProperties(pugi::xml_node node,
                      const Tile& tile,
                      const std::filesystem::path& dir)
{
  AppendPropertiesImpl(node, tile, dir);
}

void AppendProperties(pugi::xml_node node,
                      const Object& object,
                      const std::filesystem::path& dir)
{
  AppendPropertiesImpl(node, object, dir);
}

}  // namespace Tactile::IO
