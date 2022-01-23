#include "ir_object.hpp"

#include <cassert>  // assert

#include "ir_definitions.hpp"

namespace tactile::IO {

void SetName(Object& object, const CStr name)
{
  assert(name);
  object.name = name;
}

void SetId(Object& object, const int32 id)
{
  object.id = id;
}

void SetX(Object& object, const float x)
{
  object.x = x;
}

void SetY(Object& object, const float y)
{
  object.y = y;
}

void SetWidth(Object& object, const float width)
{
  object.width = width;
}

void SetHeight(Object& object, const float height)
{
  object.height = height;
}

void SetType(Object& object, const ObjectType type)
{
  object.type = type;
}

void SetTag(Object& object, const CStr tag)
{
  assert(tag);
  object.tag = tag;
}

void SetVisible(Object& object, const bool visible)
{
  object.visible = visible;
}

void ReserveProperties(Object& object, const usize n)
{
  object.properties.reserve(n);
}

auto AddProperty(Object& object) -> Property&
{
  return object.properties.emplace_back();
}

auto GetPropertyCount(const Object& object) -> usize
{
  return object.properties.size();
}

auto GetProperty(Object& object, const usize index) -> Property&
{
  return object.properties.at(index);
}

auto GetProperty(const Object& object, const usize index) -> const Property&
{
  return object.properties.at(index);
}

auto GetName(const Object& object) -> CStr
{
  return object.name.c_str();
}

auto GetId(const Object& object) -> int32
{
  return object.id;
}

auto GetX(const Object& object) -> float
{
  return object.x;
}

auto GetY(const Object& object) -> float
{
  return object.y;
}

auto GetWidth(const Object& object) -> float
{
  return object.width;
}

auto GetHeight(const Object& object) -> float
{
  return object.height;
}

auto GetType(const Object& object) -> ObjectType
{
  return object.type;
}

auto GetTag(const Object& object) -> CStr
{
  return object.tag.c_str();
}

auto IsVisible(const Object& object) -> bool
{
  return object.visible;
}

}  // namespace tactile::IO