#include "ir_property.hpp"

#include "ir_definitions.hpp"

namespace Tactile::IO {

void SetName(Property& property, const CStr name)
{
  assert(name);
  property.name = name;
}

void AssignInt(Property& property, const int32 value)
{
  property.value.SetValue(value);
}

void AssignFloat(Property& property, const float value)
{
  property.value.SetValue(value);
}

void AssignString(Property& property, const CStr str)
{
  assert(str);
  property.value.SetValue(std::string{str});
}

void AssignBool(Property& property, const bool value)
{
  property.value.SetValue(value);
}

void AssignFile(Property& property, const CPathStr path)
{
  assert(path);
  property.value.SetValue(std::filesystem::path{path});
}

void AssignObject(Property& property, const int32 id)
{
  property.value.SetValue(ObjectRef{id});
}

void AssignColor(Property& property, const Color& color)
{
  property.value.SetValue(cen::color{color.red, color.green, color.blue, color.alpha});
}

auto GetName(const Property& property) -> CStr
{
  return property.name.c_str();
}

auto GetType(const Property& property) -> PropertyType
{
  return property.value.GetType().value();
}

auto GetInt(const Property& property) -> int32
{
  return property.value.AsInt();
}

auto GetFloat(const Property& property) -> float
{
  return property.value.AsFloat();
}

auto GetString(const Property& property) -> CStr
{
  return property.value.AsString().c_str();
}

auto GetBool(const Property& property) -> bool
{
  return property.value.AsBool();
}

auto GetFile(const Property& property) -> CPathStr
{
  return property.value.AsFile().c_str();
}

auto GetObject(const Property& property) -> int32
{
  return property.value.AsObject().get();
}

auto GetColor(const Property& property) -> Color
{
  const auto& color = property.value.AsColor();
  return {color.red(), color.green(), color.blue(), color.alpha()};
}

}  // namespace Tactile::IO
