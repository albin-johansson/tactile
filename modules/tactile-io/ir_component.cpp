#include "ir_component.hpp"

#include <variant>  // holds_alternative

#include <tactile_stdlib.hpp>

#include "ir_definitions.hpp"

namespace Tactile::IO {
namespace {

template <typename T>
[[nodiscard]] auto AddComponentImpl(T& target, const ComponentDef& def) -> Component&
{
  auto& component = target.components.emplace_back();
  component.type = def.name;
  component.values = def.attributes;
  return component;
}

template <typename T, typename U>
[[nodiscard]] auto GetAttributeValue(const U& attributes, const CStr attr) -> const T&
{
  if (const auto iter = attributes.find(attr); iter != attributes.end()) {
    if (const auto* ptr = std::get_if<T>(&iter->second)) {
      return *ptr;
    }
    else {
      throw TactileError{"Attribute type mismatch!"};
    }
  }
  else {
    throw TactileError{"Did not find attribute!"};
  }
}

template <typename T>
[[nodiscard]] auto GetAttributeTypeImpl(const T& attributes, const CStr attribute)
    -> PropertyType
{
  if (const auto iter = attributes.find(attribute); iter != attributes.end()) {
    const auto& value = iter->second;
    if (std::holds_alternative<int32>(value)) {
      return PropertyType::Integer;
    }
    else if (std::holds_alternative<float>(value)) {
      return PropertyType::Floating;
    }
    else if (std::holds_alternative<bool>(value)) {
      return PropertyType::Boolean;
    }
    else if (std::holds_alternative<std::filesystem::path>(value)) {
      return PropertyType::File;
    }
    else if (std::holds_alternative<Color>(value)) {
      return PropertyType::Color;
    }
    else if (std::holds_alternative<ObjectRef>(value)) {
      return PropertyType::Object;
    }
    else {
      return PropertyType::String;
    }
  }
  else {
    throw TactileError{"Could not deduce type of attribute with invalid name!"};
  }
}

}  // namespace

auto DefineComponent(Map& map, const CStr name) -> ComponentDef&
{
  auto& def = map.component_definitions.emplace_back();
  def.name = name;
  return def;
}

void DefineAttribute(ComponentDef& def, const CStr name, const PropertyType type)
{
  auto& attribute = def.attributes[name];
  switch (type) {
    case PropertyType::String:
      attribute.emplace<std::string>();
      break;

    case PropertyType::Integer:
      attribute.emplace<int32>();
      break;

    case PropertyType::Floating:
      attribute.emplace<float>();
      break;

    case PropertyType::Boolean:
      attribute.emplace<bool>();
      break;

    case PropertyType::File:
      attribute.emplace<std::filesystem::path>();
      break;

    case PropertyType::Color:
      attribute.emplace<Color>(Color{0, 0, 0, 0xFF});
      break;

    case PropertyType::Object:
      attribute.emplace<ObjectRef>();
      break;
  }
}

void SetAttributeDefaultString(ComponentDef& def, const CStr attribute, const CStr str)
{
  if (const auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    iter->second.emplace<std::string>(str);
  }
}

void SetAttributeDefaultFile(ComponentDef& def, const CStr attribute, const CPathStr path)
{
  if (const auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    iter->second.emplace<std::filesystem::path>(path);
  }
}

void SetAttributeDefaultInt(ComponentDef& def, const CStr attribute, const int32 value)
{
  if (const auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    iter->second.emplace<int32>(value);
  }
}

void SetAttributeDefaultObject(ComponentDef& def, const CStr attribute, const int32 id)
{
  if (const auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    iter->second.emplace<ObjectRef>(ObjectRef{id});
  }
}

void SetAttributeDefaultFloat(ComponentDef& def, const CStr attribute, const float value)
{
  if (const auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    iter->second.emplace<float>(value);
  }
}

void SetAttributeDefaultBool(ComponentDef& def, const CStr attribute, const bool value)
{
  if (const auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    iter->second.emplace<bool>(value);
  }
}

void SetAttributeDefaultColor(ComponentDef& def, const CStr attribute, const Color value)
{
  if (const auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    iter->second.emplace<Color>(value);
  }
}

auto GetAttributeType(const ComponentDef& def, const CStr attribute) -> PropertyType
{
  return GetAttributeTypeImpl(def.attributes, attribute);
}

auto GetAttributeType(const Component& component, const CStr attribute) -> PropertyType
{
  return GetAttributeTypeImpl(component.values, attribute);
}

auto IsAttributeString(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeType(def, attribute) == PropertyType::String;
}

auto IsAttributeFile(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeType(def, attribute) == PropertyType::File;
}

auto IsAttributeInt(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeType(def, attribute) == PropertyType::Integer;
}

auto IsAttributeFloat(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeType(def, attribute) == PropertyType::Floating;
}

auto IsAttributeBool(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeType(def, attribute) == PropertyType::Boolean;
}

auto IsAttributeColor(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeType(def, attribute) == PropertyType::Color;
}

auto IsAttributeObject(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeType(def, attribute) == PropertyType::Object;
}

void SetName(ComponentDef& def, const CStr name)
{
  def.name = name;
}

auto GetName(const ComponentDef& def) -> CStr
{
  return def.name.c_str();
}

auto GetAttributeName(const ComponentDef& def, const usize index) -> CStr
{
  usize i = 0;

  for (const auto& [name, value] : def.attributes) {
    if (i == index) {
      return name.c_str();
    }

    ++i;
  }

  return nullptr;
}

auto GetAttributeCount(const ComponentDef& def) -> usize
{
  return def.attributes.size();
}

auto GetComponent(const Map& map, const usize index) -> const Component&
{
  return map.components.at(index);
}

auto GetComponent(const Tileset& tileset, const usize index) -> const Component&
{
  return tileset.components.at(index);
}

auto GetComponent(const Layer& layer, const usize index) -> const Component&
{
  return layer.components.at(index);
}

auto GetComponent(const Object& object, const usize index) -> const Component&
{
  return object.components.at(index);
}

auto GetComponent(const Tile& tile, const usize index) -> const Component&
{
  return tile.components.at(index);
}

auto GetComponentCount(const Tileset& tileset) -> usize
{
  return tileset.components.size();
}

auto GetComponentCount(const Layer& layer) -> usize
{
  return layer.components.size();
}

auto GetComponentCount(const Object& object) -> usize
{
  return object.components.size();
}

auto GetComponentCount(const Tile& tile) -> usize
{
  return tile.components.size();
}

auto GetComponentDef(Map& map, const CStr name) -> ComponentDef&
{
  for (auto& def : map.component_definitions) {
    if (def.name == name) {
      return def;
    }
  }

  throw TactileError{"Failed to find component definition!"};
}

auto GetComponentDef(const Map& map, const CStr name) -> const ComponentDef&
{
  for (const auto& def : map.component_definitions) {
    if (def.name == name) {
      return def;
    }
  }

  throw TactileError{"Failed to find component definition!"};
}

auto GetComponentDef(const Map& map, const usize index) -> const ComponentDef&
{
  return map.component_definitions.at(index);
}

auto GetComponentDefCount(const Map& map) -> usize
{
  return map.component_definitions.size();
}

void ReserveComponents(Map& map, const usize n)
{
  map.components.reserve(n);
}

void ReserveComponents(Tileset& tileset, const usize n)
{
  tileset.components.reserve(n);
}

void ReserveComponents(Layer& layer, const usize n)
{
  layer.components.reserve(n);
}

void ReserveComponents(Object& object, const usize n)
{
  object.components.reserve(n);
}

void ReserveComponents(Tile& tile, const usize n)
{
  tile.components.reserve(n);
}

auto AddComponent(Map& map, const ComponentDef& def) -> Component&
{
  return AddComponentImpl(map, def);
}

auto AddComponent(Tileset& tileset, const ComponentDef& def) -> Component&
{
  return AddComponentImpl(tileset, def);
}

auto AddComponent(Layer& layer, const ComponentDef& def) -> Component&
{
  return AddComponentImpl(layer, def);
}

auto AddComponent(Object& object, const ComponentDef& def) -> Component&
{
  return AddComponentImpl(object, def);
}

auto AddComponent(Tile& tile, const ComponentDef& def) -> Component&
{
  return AddComponentImpl(tile, def);
}

void SetAttributeString(Component& component, const CStr attribute, const CStr value)
{
  component.values.at(attribute).emplace<std::string>(value);
}

void SetAttributeFile(Component& component, const CStr attribute, const CPathStr value)
{
  component.values.at(attribute).emplace<std::filesystem::path>(value);
}

void SetAttributeInt(Component& component, const CStr attribute, const int32 value)
{
  component.values.at(attribute).emplace<int32>(value);
}

void SetAttributeFloat(Component& component, const CStr attribute, const float value)
{
  component.values.at(attribute).emplace<float>(value);
}

void SetAttributeBool(Component& component, const CStr attribute, const bool value)
{
  component.values.at(attribute).emplace<bool>(value);
}

void SetAttributeColor(Component& component, const CStr attribute, const Color& value)
{
  component.values.at(attribute).emplace<Color>(value);
}

void SetAttributeObject(Component& component, const CStr attribute, const int32 id)
{
  component.values.at(attribute).emplace<ObjectRef>(ObjectRef{id});
}

auto GetInt(const ComponentDef& def, const CStr attribute) -> int32
{
  return GetAttributeValue<int32>(def.attributes, attribute);
}

auto GetInt(const Component& component, const CStr attribute) -> int32
{
  return GetAttributeValue<int32>(component.values, attribute);
}

auto GetFloat(const ComponentDef& def, const CStr attribute) -> float
{
  return GetAttributeValue<float>(def.attributes, attribute);
}

auto GetFloat(const Component& component, const CStr attribute) -> float
{
  return GetAttributeValue<float>(component.values, attribute);
}

auto GetBool(const ComponentDef& def, const CStr attribute) -> bool
{
  return GetAttributeValue<bool>(def.attributes, attribute);
}

auto GetBool(const Component& component, const CStr attribute) -> bool
{
  return GetAttributeValue<bool>(component.values, attribute);
}

auto GetString(const ComponentDef& def, const CStr attribute) -> CStr
{
  return GetAttributeValue<std::string>(def.attributes, attribute).c_str();
}

auto GetString(const Component& component, const CStr attribute) -> CStr
{
  return GetAttributeValue<std::string>(component.values, attribute).c_str();
}

auto GetFile(const ComponentDef& def, const CStr attribute) -> CPathStr
{
  return GetAttributeValue<std::filesystem::path>(def.attributes, attribute).c_str();
}

auto GetFile(const Component& component, const CStr attribute) -> CPathStr
{
  return GetAttributeValue<std::filesystem::path>(component.values, attribute).c_str();
}

auto GetColor(const ComponentDef& def, const CStr attribute) -> Color
{
  return GetAttributeValue<Color>(def.attributes, attribute);
}

auto GetColor(const Component& component, CStr attribute) -> Color
{
  return GetAttributeValue<Color>(component.values, attribute);
}

auto GetObject(const ComponentDef& def, const CStr attribute) -> int32
{
  return GetAttributeValue<ObjectRef>(def.attributes, attribute);
}

auto GetObject(const Component& component, const CStr attribute) -> int32
{
  return GetAttributeValue<ObjectRef>(component.values, attribute);
}

auto GetName(const Component& component) -> CStr
{
  return component.type.c_str();
}

auto GetAttributeCount(const Component& component) -> usize
{
  return component.values.size();
}

auto HasDefaultValue(const ComponentDef& def, const CStr attribute) -> bool
{
  for (const auto& [name, value] : def.attributes) {
    if (name == attribute) {
      if (const auto* str = std::get_if<std::string>(&value)) {
        return str->empty();
      }
      else if (const auto* i = std::get_if<int32>(&value)) {
        return *i == 0;
      }
      else if (const auto* f = std::get_if<float>(&value)) {
        return *f == 0;
      }
      else if (const auto* b = std::get_if<bool>(&value)) {
        return !*b;
      }
      else if (const auto* path = std::get_if<std::filesystem::path>(&value)) {
        return path->empty();
      }
      else if (const auto* obj = std::get_if<ObjectRef>(&value)) {
        return *obj == ObjectRef{};
      }
      else if (const auto* color = std::get_if<Color>(&value)) {
        return color->red == 0 && color->green == 0 && color->blue == 0 &&
               color->alpha == 0xFF;
      }
    }
  }

  throw TactileError{"Could not determine if definition attribute uses default value!"};
}

auto GetAttributeName(const Component& component, usize index) -> CStr
{
  usize i = 0;
  for (const auto& [name, value] : component.values) {
    if (i == index) {
      return name.c_str();
    }

    ++i;
  }

  return nullptr;
}

auto GetComponentCount(const Map& map) -> usize
{
  return map.components.size();
}

}  // namespace Tactile::IO