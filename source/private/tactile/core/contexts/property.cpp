/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tactile/core/contexts/property.hpp"

#include <type_traits>  // decay_t
#include <utility>      // to_underlying
#include <variant>      // visit

#include "tactile/core/common/error.hpp"

namespace tactile {

Property::Property(const PropertyType type)
{
  reset(type);
}

void Property::reset(const PropertyType type)
{
  switch (type) {
    case PropertyType::Str:
      set(string_type {});
      break;

    case PropertyType::Int:
      set(int_type {});
      break;

    case PropertyType::Int2:
      set(int2_type {});
      break;

    case PropertyType::Int3:
      set(int3_type {});
      break;

    case PropertyType::Int4:
      set(int4_type {});
      break;

    case PropertyType::Float:
      set(float_type {});
      break;

    case PropertyType::Float2:
      set(float2_type {});
      break;

    case PropertyType::Float3:
      set(float3_type {});
      break;

    case PropertyType::Float4:
      set(float4_type {});
      break;

    case PropertyType::Bool:
      set(bool {});
      break;

    case PropertyType::Path:
      set(path_type {});
      break;

    case PropertyType::Color:
      set(color_type {});
      break;

    case PropertyType::Object:
      set(objref_type {});
      break;

    default:
      throw Error {"Invalid property type"};
  }
}

auto Property::as_string() const -> const string_type&
{
  if (const auto* str = _get_if<string_type>()) {
    return *str;
  }
  else {
    throw Error {"Property was not a string"};
  }
}

auto Property::as_int() const -> int_type
{
  if (const auto* i = _get_if<int_type>()) {
    return *i;
  }
  else {
    throw Error {"Property was not an int"};
  }
}

auto Property::as_int2() const -> const int2_type&
{
  if (const auto* vec = _get_if<int2_type>()) {
    return *vec;
  }
  else {
    throw Error {"Property was not a 2D int"};
  }
}

auto Property::as_int3() const -> const int3_type&
{
  if (const auto* vec = _get_if<int3_type>()) {
    return *vec;
  }
  else {
    throw Error {"Property was not a 3D int"};
  }
}

auto Property::as_int4() const -> const int4_type&
{
  if (const auto* vec = _get_if<int4_type>()) {
    return *vec;
  }
  else {
    throw Error {"Property was not a 4D int"};
  }
}

auto Property::as_float() const -> float_type
{
  if (const auto* f = _get_if<float_type>()) {
    return *f;
  }
  else {
    throw Error {"Property was not a float"};
  }
}

auto Property::as_float2() const -> const float2_type&
{
  if (const auto* vec = _get_if<float2_type>()) {
    return *vec;
  }
  else {
    throw Error {"Property was not a 2D float"};
  }
}

auto Property::as_float3() const -> const float3_type&
{
  if (const auto* vec = _get_if<float3_type>()) {
    return *vec;
  }
  else {
    throw Error {"Property was not a 3D float"};
  }
}

auto Property::as_float4() const -> const float4_type&
{
  if (const auto* vec = _get_if<float4_type>()) {
    return *vec;
  }
  else {
    throw Error {"Property was not a 4D float"};
  }
}

auto Property::as_bool() const -> bool
{
  if (const auto* b = _get_if<bool>()) {
    return *b;
  }
  else {
    throw Error {"Property was not a bool"};
  }
}

auto Property::as_color() const -> const color_type&
{
  if (const auto* c = _get_if<color_type>()) {
    return *c;
  }
  else {
    throw Error {"Property was not a color"};
  }
}

auto Property::as_path() const -> const path_type&
{
  if (const auto* path = _get_if<path_type>()) {
    return *path;
  }
  else {
    throw Error {"Property was not a path"};
  }
}

auto Property::as_object() const -> objref_type
{
  if (const auto* obj = _get_if<objref_type>()) {
    return *obj;
  }
  else {
    throw Error {"Property was not an object reference"};
  }
}

auto Property::get_type() const -> PropertyType
{
  switch (mValue.index()) {
    case kStringTypeIndex:
      return PropertyType::Str;

    case kIntTypeIndex:
      return PropertyType::Int;

    case kInt2TypeIndex:
      return PropertyType::Int2;

    case kInt3TypeIndex:
      return PropertyType::Int3;

    case kInt4TypeIndex:
      return PropertyType::Int4;

    case kFloatTypeIndex:
      return PropertyType::Float;

    case kFloat2TypeIndex:
      return PropertyType::Float2;

    case kFloat3TypeIndex:
      return PropertyType::Float3;

    case kFloat4TypeIndex:
      return PropertyType::Float4;

    case kBoolTypeIndex:
      return PropertyType::Bool;

    case kColorTypeIndex:
      return PropertyType::Color;

    case kPathTypeIndex:
      return PropertyType::Path;

    case kObjRefTypeIndex:
      return PropertyType::Object;

    default:
      throw Error {"Invalid property type"};
  }
}

auto Property::is_vector() const -> bool
{
  switch (get_type()) {
    case PropertyType::Int2:
    case PropertyType::Int3:
    case PropertyType::Int4:
    case PropertyType::Float2:
    case PropertyType::Float3:
    case PropertyType::Float4:
      return true;

    default:
      return false;
  }
}

auto Property::has_initial_value() const -> bool
{
  auto visitor = [](auto&& value) -> bool {
    using T = std::decay_t<decltype(value)>;
    return value == T {};
  };

  return std::visit(visitor, mValue);
}

auto operator<<(std::ostream& stream, const Property& property) -> std::ostream&
{
  switch (property.get_type()) {
    case PropertyType::Str:
      return stream << property.as_string();

    case PropertyType::Int:
      return stream << property.as_int();

    case PropertyType::Int2: {
      const auto& vec = property.as_int2();
      return stream << '{' << vec.x << ", " << vec.y << '}';
    }
    case PropertyType::Int3: {
      const auto& vec = property.as_int3();
      return stream << '{' << vec.x << ", " << vec.y << ", " << vec.z << '}';
    }
    case PropertyType::Int4: {
      const auto& vec = property.as_int4();
      return stream << '{' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
                    << '}';
    }
    case PropertyType::Float:
      return stream << property.as_float();

    case PropertyType::Float2: {
      const auto& vec = property.as_float2();
      return stream << '{' << vec.x << ", " << vec.y << '}';
    }
    case PropertyType::Float3: {
      const auto& vec = property.as_float3();
      return stream << '{' << vec.x << ", " << vec.y << ", " << vec.z << '}';
    }
    case PropertyType::Float4: {
      const auto& vec = property.as_float4();
      return stream << '{' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
                    << '}';
    }
    case PropertyType::Bool:
      return stream << (property.as_bool() ? "true" : "false");

    case PropertyType::Path:
      return stream << property.as_path();

    case PropertyType::Color:
      return stream << to_string_rgba(property.as_color());

    case PropertyType::Object:
      return stream << std::to_underlying(property.as_object());
  }

  return stream;
}

auto index_of_varying_vector_dimension(const Property& a, const Property& b)
    -> Maybe<usize>
{
  auto compare = [](const auto& v1, const auto& v2) -> Maybe<usize> {
    const auto mask = compare_vector_components(v1, v2);

    if (!(mask & kVectorXBit)) {
      return 0;
    }
    else if (!(mask & kVectorYBit)) {
      return 1;
    }
    else if (!(mask & kVectorZBit)) {
      return 2;
    }
    else if (!(mask & kVectorWBit)) {
      return 3;
    }

    return kNone;
  };

  if (a.is_vector() && a.get_type() == b.get_type()) {
    switch (a.get_type()) {
      case PropertyType::Int2:
        return compare(a.as_int2(), b.as_int2());

      case PropertyType::Int3:
        return compare(a.as_int3(), b.as_int3());

      case PropertyType::Int4:
        return compare(a.as_int4(), b.as_int4());

      case PropertyType::Float2:
        return compare(a.as_float2(), b.as_float2());

      case PropertyType::Float3:
        return compare(a.as_float3(), b.as_float3());

      case PropertyType::Float4:
        return compare(a.as_float4(), b.as_float4());

      default:
        break;
    }
  }

  return kNone;
}

}  // namespace tactile
