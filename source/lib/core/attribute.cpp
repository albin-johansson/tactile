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

#include "attribute.hpp"

#include <fmt/format.h>

#include "common/util/fmt.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

void Attribute::reset_to_default(const AttributeType type)
{
  switch (type) {
    case AttributeType::kStr:
      set_value<string_type>(string_type {});
      break;

    case AttributeType::kInt:
      set_value<int_type>(0);
      break;

    case AttributeType::kInt2:
      set_value<int2_type>(int2_type {0, 0});
      break;

    case AttributeType::kInt3:
      set_value<int3_type>(int3_type {0, 0, 0});
      break;

    case AttributeType::kInt4:
      set_value<int4_type>(int4_type {0, 0, 0, 0});
      break;

    case AttributeType::kFloat:
      set_value<float_type>(0);
      break;

    case AttributeType::kFloat2:
      set_value<float2_type>(float2_type {0, 0});
      break;

    case AttributeType::kFloat3:
      set_value<float3_type>(float3_type {0, 0, 0});
      break;

    case AttributeType::kFloat4:
      set_value<float4_type>(float4_type {0, 0, 0, 0});
      break;

    case AttributeType::kBool:
      set_value<bool>(false);
      break;

    case AttributeType::kPath:
      set_value<path_type>(path_type {});
      break;

    case AttributeType::kColor:
      set_value<color_type>(kBlack);
      break;

    case AttributeType::kObject:
      set_value<ObjectRef>(ObjectRef {});
      break;

    default:
      throw Exception {"Invalid attribute type!"};
  }
}

auto Attribute::has_default_value() const -> bool
{
  if (const auto* str = try_as_string()) {
    return str->empty();
  }
  else if (const auto* i = try_as_int()) {
    return *i == 0;
  }
  else if (const auto* i2 = try_as_int2()) {
    return *i2 == int2_type {};
  }
  else if (const auto* i3 = try_as_int3()) {
    return *i3 == int3_type {};
  }
  else if (const auto* i4 = try_as_int4()) {
    return *i4 == int4_type {};
  }
  else if (const auto* f = try_as_float()) {
    return *f == 0;
  }
  else if (const auto* f2 = try_as_float2()) {
    return *f2 == float2_type {};
  }
  else if (const auto* f3 = try_as_float3()) {
    return *f3 == float3_type {};
  }
  else if (const auto* f4 = try_as_float4()) {
    return *f4 == float4_type {};
  }
  else if (const auto* b = try_as_bool()) {
    return !*b;
  }
  else if (const auto* path = try_as_path()) {
    return path->empty();
  }
  else if (const auto* obj = try_as_object()) {
    return *obj == ObjectRef {};
  }
  else if (const auto* color = try_as_color()) {
    return *color == kBlack;
  }
  else {
    throw Exception {"Invalid property type"};
  }
}

auto Attribute::get_type() const -> AttributeType
{
  switch (mValue.index()) {
    case kStringTypeIndex:
      return AttributeType::kStr;

    case kIntTypeIndex:
      return AttributeType::kInt;

    case kInt2TypeIndex:
      return AttributeType::kInt2;

    case kInt3TypeIndex:
      return AttributeType::kInt3;

    case kInt4TypeIndex:
      return AttributeType::kInt4;

    case kFloatTypeIndex:
      return AttributeType::kFloat;

    case kFloat2TypeIndex:
      return AttributeType::kFloat2;

    case kFloat3TypeIndex:
      return AttributeType::kFloat3;

    case kFloat4TypeIndex:
      return AttributeType::kFloat4;

    case kBoolTypeIndex:
      return AttributeType::kBool;

    case kColorTypeIndex:
      return AttributeType::kColor;

    case kPathTypeIndex:
      return AttributeType::kPath;

    case kObjRefTypeIndex:
      return AttributeType::kObject;

    default:
      throw Exception {"Invalid attribute type"};
  }
}

auto Attribute::is_string() const -> bool
{
  return get_type() == AttributeType::kStr;
}

auto Attribute::is_int() const -> bool
{
  return get_type() == AttributeType::kInt;
}

auto Attribute::is_int2() const -> bool
{
  return get_type() == AttributeType::kInt2;
}

auto Attribute::is_int3() const -> bool
{
  return get_type() == AttributeType::kInt3;
}

auto Attribute::is_int4() const -> bool
{
  return get_type() == AttributeType::kInt4;
}

auto Attribute::is_float() const -> bool
{
  return get_type() == AttributeType::kFloat;
}

auto Attribute::is_float2() const -> bool
{
  return get_type() == AttributeType::kFloat2;
}

auto Attribute::is_float3() const -> bool
{
  return get_type() == AttributeType::kFloat3;
}

auto Attribute::is_float4() const -> bool
{
  return get_type() == AttributeType::kFloat4;
}

auto Attribute::is_bool() const -> bool
{
  return get_type() == AttributeType::kBool;
}

auto Attribute::is_path() const -> bool
{
  return get_type() == AttributeType::kPath;
}

auto Attribute::is_object() const -> bool
{
  return get_type() == AttributeType::kObject;
}

auto Attribute::is_color() const -> bool
{
  return get_type() == AttributeType::kColor;
}

auto Attribute::is_any_vector() const -> bool
{
  switch (get_type()) {
    case AttributeType::kInt2:
    case AttributeType::kInt3:
    case AttributeType::kInt4:
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4:
      return true;

    default:
      return false;
  }
}

auto Attribute::as_string() const -> const string_type&
{
  if (const auto* str = get_if<string_type>()) {
    return *str;
  }
  else {
    throw Exception {"Attribute was not a string"};
  }
}

auto Attribute::as_int() const -> int_type
{
  if (const auto* i = get_if<int_type>()) {
    return *i;
  }
  else {
    throw Exception {"Attribute was not an int"};
  }
}

auto Attribute::as_int2() const -> const int2_type&
{
  if (const auto* i2 = get_if<int2_type>()) {
    return *i2;
  }
  else {
    throw Exception {"Attribute was not an int2"};
  }
}

auto Attribute::as_int3() const -> const int3_type&
{
  if (const auto* i3 = get_if<int3_type>()) {
    return *i3;
  }
  else {
    throw Exception {"Attribute was not an int3"};
  }
}

auto Attribute::as_int4() const -> const int4_type&
{
  if (const auto* i4 = get_if<int4_type>()) {
    return *i4;
  }
  else {
    throw Exception {"Attribute was not an int4"};
  }
}

auto Attribute::as_float() const -> float_type
{
  if (const auto* f = get_if<float_type>()) {
    return *f;
  }
  else {
    throw Exception {"Attribute was not a float"};
  }
}

auto Attribute::as_float2() const -> const float2_type&
{
  if (const auto* f2 = get_if<float2_type>()) {
    return *f2;
  }
  else {
    throw Exception {"Attribute was not a float2"};
  }
}

auto Attribute::as_float3() const -> const float3_type&
{
  if (const auto* f3 = get_if<float3_type>()) {
    return *f3;
  }
  else {
    throw Exception {"Attribute was not a float3"};
  }
}

auto Attribute::as_float4() const -> const float4_type&
{
  if (const auto* f4 = get_if<float4_type>()) {
    return *f4;
  }
  else {
    throw Exception {"Attribute was not a float4"};
  }
}

auto Attribute::as_bool() const -> bool
{
  if (const auto* b = get_if<bool>()) {
    return *b;
  }
  else {
    throw Exception {"Attribute was not a bool"};
  }
}

auto Attribute::as_path() const -> const path_type&
{
  if (const auto* file = get_if<path_type>()) {
    return *file;
  }
  else {
    throw Exception {"Attribute was not a path"};
  }
}

auto Attribute::as_object() const -> ObjectRef
{
  if (const auto* obj = get_if<ObjectRef>()) {
    return *obj;
  }
  else {
    throw Exception {"Attribute was not an object reference"};
  }
}

auto Attribute::as_color() const -> const color_type&
{
  if (const auto* color = get_if<color_type>()) {
    return *color;
  }
  else {
    throw Exception {"Attribute was not a color"};
  }
}

auto parse_attr_type(StringView name) -> Maybe<AttributeType>
{
  if (name == "string") {
    return AttributeType::kStr;
  }
  else if (name == "int") {
    return AttributeType::kInt;
  }
  else if (name == "int2") {
    return AttributeType::kInt2;
  }
  else if (name == "int3") {
    return AttributeType::kInt3;
  }
  else if (name == "int4") {
    return AttributeType::kInt4;
  }
  else if (name == "float") {
    return AttributeType::kFloat;
  }
  else if (name == "float2") {
    return AttributeType::kFloat2;
  }
  else if (name == "float3") {
    return AttributeType::kFloat3;
  }
  else if (name == "float4") {
    return AttributeType::kFloat4;
  }
  else if (name == "bool") {
    return AttributeType::kBool;
  }
  else if (name == "file" || name == "path") {
    return AttributeType::kPath;
  }
  else if (name == "color") {
    return AttributeType::kColor;
  }
  else if (name == "object") {
    return AttributeType::kObject;
  }
  else {
    return nothing;
  }
}

auto serialize_to_save_format(const AttributeType type) -> StringView
{
  switch (type) {
    case AttributeType::kStr:
      return "string";

    case AttributeType::kInt:
      return "int";

    case AttributeType::kInt2:
      return "int2";

    case AttributeType::kInt3:
      return "int3";

    case AttributeType::kInt4:
      return "int4";

    case AttributeType::kFloat:
      return "float";

    case AttributeType::kFloat2:
      return "float2";

    case AttributeType::kFloat3:
      return "float3";

    case AttributeType::kFloat4:
      return "float4";

    case AttributeType::kBool:
      return "bool";

    case AttributeType::kPath:
      return "file";

    case AttributeType::kColor:
      return "color";

    case AttributeType::kObject:
      return "object";

    default:
      throw Exception {"Invalid attribute type"};
  }
}

auto serialize_to_save_format(const Int2& vec) -> String
{
  return fmt::format("{};{}", vec.x, vec.y);
}

auto serialize_to_save_format(const Float2& vec) -> String
{
  return fmt::format("{};{}", vec.x, vec.y);
}

auto serialize_to_save_format(const Int3& vec) -> String
{
  return fmt::format("{};{};{}", vec.x, vec.y, vec.z);
}

auto serialize_to_save_format(const Float3& vec) -> String
{
  return fmt::format("{};{};{}", vec.x, vec.y, vec.z);
}

auto serialize_to_save_format(const Int4& vec) -> String
{
  return fmt::format("{};{};{};{}", vec.x, vec.y, vec.z, vec.w);
}

auto serialize_to_save_format(const Float4& vec) -> String
{
  return fmt::format("{};{};{};{}", vec.x, vec.y, vec.z, vec.w);
}

auto operator<<(std::ostream& stream, const AttributeType type) -> std::ostream&
{
  return stream << serialize_to_save_format(type);
}

auto operator<<(std::ostream& stream, const Attribute& value) -> std::ostream&
{
  switch (value.get_type()) {
    case AttributeType::kStr:
      return stream << value.as_string();

    case AttributeType::kInt:
      return stream << value.as_int();

    case AttributeType::kInt2:
      return stream << value.as_int2();

    case AttributeType::kInt3:
      return stream << value.as_int3();

    case AttributeType::kInt4:
      return stream << value.as_int4();

    case AttributeType::kFloat:
      return stream << value.as_float();

    case AttributeType::kFloat2:
      return stream << value.as_float2();

    case AttributeType::kFloat3:
      return stream << value.as_float3();

    case AttributeType::kFloat4:
      return stream << value.as_float4();

    case AttributeType::kBool:
      return stream << value.as_bool();

    case AttributeType::kPath:
      return stream << value.as_path();

    case AttributeType::kColor:
      return stream << value.as_color().as_rgba();

    case AttributeType::kObject:
      return stream << "object/" << value.as_object();

    default:
      throw Exception {"Invalid attribute type"};
  }
}

}  // namespace tactile
