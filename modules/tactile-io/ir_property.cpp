#include "ir_property.hpp"

#include <cassert>     // assert
#include <filesystem>  // path
#include <string>      // string
#include <variant>     // get, holds_alternative

#include <tactile_stdlib.hpp>

#include "ir_definitions.hpp"

namespace tactile::IO {

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
  if (IsInt(property)) {
    return PropertyType::Integer;
  }
  else if (IsFloat(property)) {
    return PropertyType::Floating;
  }
  else if (IsBool(property)) {
    return PropertyType::Boolean;
  }
  else if (IsColor(property)) {
    return PropertyType::Color;
  }
  else if (IsObject(property)) {
    return PropertyType::Object;
  }
  else if (IsFile(property)) {
    return PropertyType::File;
  }
  else /*if (IsString(property))*/ {
    return PropertyType::String;
  }
}

auto GetInt(const Property& property) -> int32
{
  try {
    return std::get<int32>(property.value);
  }
  catch (...) {
    throw TactileError{"Attempted to get integer value of non-integral property!"};
  }
}

auto GetFloat(const Property& property) -> float
{
  try {
    return std::get<float>(property.value);
  }
  catch (...) {
    throw TactileError{"Attempted to get float value of non-floating property!"};
  }
}

auto GetString(const Property& property) -> CStr
{
  try {
    return std::get<std::string>(property.value).c_str();
  }
  catch (...) {
    throw TactileError{"Attempted to get string value of non-string property!"};
  }
}

auto GetBool(const Property& property) -> bool
{
  try {
    return std::get<bool>(property.value);
  }
  catch (...) {
    throw TactileError{"Attempted to get boolean value of non-boolean property!"};
  }
}

auto GetFile(const Property& property) -> CPathStr
{
  try {
    return std::get<std::filesystem::path>(property.value).c_str();
  }
  catch (...) {
    throw TactileError{"Attempted to get file path value of non-file property!"};
  }
}

auto GetObject(const Property& property) -> int32
{
  try {
    return std::get<ObjectRef>(property.value);
  }
  catch (...) {
    throw TactileError{"Attempted to get object value of non-object property!"};
  }
}

auto GetColor(const Property& property) -> Color
{
  try {
    return std::get<Color>(property.value);
  }
  catch (...) {
    throw TactileError{"Attempted to get color value of non-color property!"};
  }
}

auto IsInt(const Property& property) -> bool
{
  return std::holds_alternative<int32>(property.value);
}

auto IsFloat(const Property& property) -> bool
{
  return std::holds_alternative<float>(property.value);
}

auto IsString(const Property& property) -> bool
{
  return std::holds_alternative<std::string>(property.value);
}

auto IsBool(const Property& property) -> bool
{
  return std::holds_alternative<bool>(property.value);
}

auto IsFile(const Property& property) -> bool
{
  return std::holds_alternative<std::filesystem::path>(property.value);
}

auto IsObject(const Property& property) -> bool
{
  return std::holds_alternative<ObjectRef>(property.value);
}

auto IsColor(const Property& property) -> bool
{
  return std::holds_alternative<Color>(property.value);
}

}  // namespace tactile::IO
