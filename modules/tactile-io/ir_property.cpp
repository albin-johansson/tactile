#include "ir_property.hpp"

#include <cassert>     // assert
#include <filesystem>  // path
#include <string>      // string
#include <variant>     // get, holds_alternative

#include "ir_definitions.hpp"

namespace Tactile::IO {

void SetName(Property& property, const CStr name)
{
  assert(name);
  property.name = name;
}

void AssignInt(Property& property, const int32 value)
{
  property.value.emplace<int32>(value);
}

void AssignFloat(Property& property, const float value)
{
  property.value.emplace<float>(value);
}

void AssignString(Property& property, const CStr str)
{
  assert(str);
  property.value.emplace<std::string>(str);
}

void AssignBool(Property& property, const bool value)
{
  property.value.emplace<bool>(value);
}

void AssignFile(Property& property, const CPathStr path)
{
  assert(path);
  property.value.emplace<std::filesystem::path>(path);
}

void AssignObject(Property& property, const int32 id)
{
  property.value.emplace<ObjectRef>(ObjectRef{id});
}

void AssignColor(Property& property, const Color& color)
{
  property.value.emplace<Color>(color);
}

auto GetName(const Property& property) -> CStr
{
  return property.name.c_str();
}

auto GetType(const Property& property) -> PropertyType
{
  if (std::holds_alternative<int32>(property.value)) {
    return PropertyType::Integer;
  }
  else if (std::holds_alternative<float>(property.value)) {
    return PropertyType::Floating;
  }
  else if (std::holds_alternative<bool>(property.value)) {
    return PropertyType::Boolean;
  }
  else if (std::holds_alternative<Color>(property.value)) {
    return PropertyType::Color;
  }
  else if (std::holds_alternative<ObjectRef>(property.value)) {
    return PropertyType::Object;
  }
  else if (std::holds_alternative<std::filesystem::path>(property.value)) {
    return PropertyType::File;
  }
  else /*if (std::holds_alternative<std::string>(property.value))*/ {
    return PropertyType::String;
  }
}

auto GetInt(const Property& property) -> int32
{
  return std::get<int32>(property.value);
}

auto GetFloat(const Property& property) -> float
{
  return std::get<float>(property.value);
}

auto GetString(const Property& property) -> CStr
{
  return std::get<std::string>(property.value).c_str();
}

auto GetBool(const Property& property) -> bool
{
  return std::get<bool>(property.value);
}

auto GetFile(const Property& property) -> CPathStr
{
  return std::get<std::filesystem::path>(property.value).c_str();
}

auto GetObject(const Property& property) -> int32
{
  return std::get<ObjectRef>(property.value);
}

auto GetColor(const Property& property) -> Color
{
  return std::get<Color>(property.value);
}

}  // namespace Tactile::IO
