/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "common/debug/assert.hpp"
#include "common/debug/panic.hpp"

namespace tactile {

auto stringify(const AttributeType type) -> const char*
{
  switch (type) {
    case AttributeType::String:
      return "string";

    case AttributeType::Int:
      return "int";

    case AttributeType::Int2:
      return "int2";

    case AttributeType::Int3:
      return "int3";

    case AttributeType::Int4:
      return "int4";

    case AttributeType::Float:
      return "float";

    case AttributeType::Float2:
      return "float2";

    case AttributeType::Float3:
      return "float3";

    case AttributeType::Float4:
      return "float4";

    case AttributeType::Bool:
      return "bool";

    case AttributeType::Path:
      return "file";

    case AttributeType::Color:
      return "color";

    case AttributeType::Object:
      return "object";

    default:
      throw TactileError {"Invalid attribute type!"};
  }
}

auto parse_attr_type(StringView name) -> Maybe<AttributeType>
{
  if (name == "string") {
    return AttributeType::String;
  }
  else if (name == "int") {
    return AttributeType::Int;
  }
  else if (name == "int2") {
    return AttributeType::Int2;
  }
  else if (name == "int3") {
    return AttributeType::Int3;
  }
  else if (name == "int4") {
    return AttributeType::Int4;
  }
  else if (name == "float") {
    return AttributeType::Float;
  }
  else if (name == "float2") {
    return AttributeType::Float2;
  }
  else if (name == "float3") {
    return AttributeType::Float3;
  }
  else if (name == "float4") {
    return AttributeType::Float4;
  }
  else if (name == "bool") {
    return AttributeType::Bool;
  }
  else if (name == "file" || name == "path") {
    return AttributeType::Path;
  }
  else if (name == "color") {
    return AttributeType::Color;
  }
  else if (name == "object") {
    return AttributeType::Object;
  }
  else {
    return nothing;
  }
}

void Attribute::reset_to_default(const AttributeType type)
{
  switch (type) {
    case AttributeType::String:
      set_value<string_type>(string_type {});
      break;

    case AttributeType::Int:
      set_value<integer_type>(0);
      break;

    case AttributeType::Int2:
      set_value<int2_type>(int2_type {0, 0});
      break;

    case AttributeType::Int3:
      set_value<int3_type>(int3_type {0, 0, 0});
      break;

    case AttributeType::Int4:
      set_value<int4_type>(int4_type {0, 0, 0, 0});
      break;

    case AttributeType::Float:
      set_value<float_type>(0);
      break;

    case AttributeType::Float2:
      set_value<float2_type>(float2_type {0, 0});
      break;

    case AttributeType::Float3:
      set_value<float3_type>(float3_type {0, 0, 0});
      break;

    case AttributeType::Float4:
      set_value<float4_type>(float4_type {0, 0, 0, 0});
      break;

    case AttributeType::Bool:
      set_value<bool>(false);
      break;

    case AttributeType::Path:
      set_value<path_type>(path_type {});
      break;

    case AttributeType::Color:
      set_value<color_type>(black);
      break;

    case AttributeType::Object:
      set_value<object_t>(object_t {});
      break;

    default:
      throw TactileError {"Invalid attribute type!"};
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
    return *i2 == int2_type {0, 0};
  }
  else if (const auto* i3 = try_as_int3()) {
    return *i3 == int3_type {0, 0, 0};
  }
  else if (const auto* i4 = try_as_int4()) {
    return *i4 == int4_type {0, 0, 0, 0};
  }
  else if (const auto* f = try_as_float()) {
    return *f == 0;
  }
  else if (const auto* f2 = try_as_float2()) {
    return *f2 == float2_type {0, 0};
  }
  else if (const auto* f3 = try_as_float3()) {
    return *f3 == float3_type {0, 0, 0};
  }
  else if (const auto* f4 = try_as_float4()) {
    return *f4 == float4_type {0, 0, 0, 0};
  }
  else if (const auto* b = try_as_bool()) {
    return !*b;
  }
  else if (const auto* path = try_as_path()) {
    return path->empty();
  }
  else if (const auto* obj = std::get_if<object_t>(&mValue)) {
    return *obj == object_t {};
  }
  else if (const auto* color = try_as_color()) {
    return *color == black;
  }
  else {
    throw TactileError {"Invalid property type"};
  }
}

auto Attribute::type() const noexcept -> AttributeType
{
  // TODO use index function instead

  if (holds<integer_type>()) {
    return AttributeType::Int;
  }
  else if (holds<int2_type>()) {
    return AttributeType::Int2;
  }
  else if (holds<int3_type>()) {
    return AttributeType::Int3;
  }
  else if (holds<int4_type>()) {
    return AttributeType::Int4;
  }
  else if (holds<float_type>()) {
    return AttributeType::Float;
  }
  else if (holds<float2_type>()) {
    return AttributeType::Float2;
  }
  else if (holds<float3_type>()) {
    return AttributeType::Float3;
  }
  else if (holds<float4_type>()) {
    return AttributeType::Float4;
  }
  else if (holds<bool>()) {
    return AttributeType::Bool;
  }
  else if (holds<object_t>()) {
    return AttributeType::Object;
  }
  else if (holds<color_type>()) {
    return AttributeType::Color;
  }
  else if (holds<path_type>()) {
    return AttributeType::Path;
  }
  else {
    TACTILE_ASSERT(holds<string_type>());
    return AttributeType::String;
  }
}

auto Attribute::is_string() const noexcept -> bool
{
  return type() == AttributeType::String;
}

auto Attribute::is_int() const noexcept -> bool
{
  return type() == AttributeType::Int;
}

auto Attribute::is_int2() const noexcept -> bool
{
  return type() == AttributeType::Int2;
}

auto Attribute::is_int3() const noexcept -> bool
{
  return type() == AttributeType::Int3;
}

auto Attribute::is_int4() const noexcept -> bool
{
  return type() == AttributeType::Int4;
}

auto Attribute::is_float() const noexcept -> bool
{
  return type() == AttributeType::Float;
}

auto Attribute::is_float2() const noexcept -> bool
{
  return type() == AttributeType::Float2;
}

auto Attribute::is_float3() const noexcept -> bool
{
  return type() == AttributeType::Float3;
}

auto Attribute::is_float4() const noexcept -> bool
{
  return type() == AttributeType::Float4;
}

auto Attribute::is_bool() const noexcept -> bool
{
  return type() == AttributeType::Bool;
}

auto Attribute::is_path() const noexcept -> bool
{
  return type() == AttributeType::Path;
}

auto Attribute::is_object() const noexcept -> bool
{
  return type() == AttributeType::Object;
}

auto Attribute::is_color() const noexcept -> bool
{
  return type() == AttributeType::Color;
}

auto Attribute::as_string() const -> const string_type&
{
  if (const auto* str = get_if<string_type>()) {
    return *str;
  }
  else {
    throw TactileError {"Attribute was not a string!"};
  }
}

auto Attribute::as_int() const -> integer_type
{
  if (const auto* i = get_if<integer_type>()) {
    return *i;
  }
  else {
    throw TactileError {"Attribute was not an integer!"};
  }
}

auto Attribute::as_int2() const -> int2_type
{
  if (const auto* i2 = get_if<int2_type>()) {
    return *i2;
  }
  else {
    throw TactileError {"Attribute was not an int2"};
  }
}

auto Attribute::as_int3() const -> int3_type
{
  if (const auto* i3 = get_if<int3_type>()) {
    return *i3;
  }
  else {
    throw TactileError {"Attribute was not an int3"};
  }
}

auto Attribute::as_int4() const -> int4_type
{
  if (const auto* i4 = get_if<int4_type>()) {
    return *i4;
  }
  else {
    throw TactileError {"Attribute was not an int4"};
  }
}

auto Attribute::as_float() const -> float_type
{
  if (const auto* f = get_if<float_type>()) {
    return *f;
  }
  else {
    throw TactileError {"Attribute was not a float!"};
  }
}

auto Attribute::as_float2() const -> float2_type
{
  if (const auto* f2 = get_if<float2_type>()) {
    return *f2;
  }
  else {
    throw TactileError {"Attribute was not a float2"};
  }
}

auto Attribute::as_float3() const -> float3_type
{
  if (const auto* f3 = get_if<float3_type>()) {
    return *f3;
  }
  else {
    throw TactileError {"Attribute was not a float3"};
  }
}

auto Attribute::as_float4() const -> float4_type
{
  if (const auto* f4 = get_if<float4_type>()) {
    return *f4;
  }
  else {
    throw TactileError {"Attribute was not a float4"};
  }
}

auto Attribute::as_bool() const -> bool
{
  if (const auto* b = get_if<bool>()) {
    return *b;
  }
  else {
    throw TactileError {"Attribute was not a boolean!"};
  }
}

auto Attribute::as_path() const -> const path_type&
{
  if (const auto* file = get_if<path_type>()) {
    return *file;
  }
  else {
    throw TactileError {"Attribute was not a file!"};
  }
}

auto Attribute::as_object() const -> object_t
{
  if (const auto* obj = get_if<object_t>()) {
    return *obj;
  }
  else {
    throw TactileError {"Attribute was not an object reference!"};
  }
}

auto Attribute::as_color() const -> const color_type&
{
  if (const auto* color = get_if<color_type>()) {
    return *color;
  }
  else {
    throw TactileError {"Attribute was not a color!"};
  }
}

auto operator<<(OStream& stream, const AttributeType type) -> OStream&
{
  return stream << stringify(type);
}

auto operator<<(OStream& stream, const Attribute& value) -> OStream&
{
  switch (value.type()) {
    case AttributeType::String:
      return stream << value.as_string();

    case AttributeType::Int:
      return stream << value.as_int();

    case AttributeType::Int2:
      return stream << value.as_int2();

    case AttributeType::Int3:
      return stream << value.as_int3();

    case AttributeType::Int4:
      return stream << value.as_int4();

    case AttributeType::Float:
      return stream << value.as_float();

    case AttributeType::Float2:
      return stream << value.as_float2();

    case AttributeType::Float3:
      return stream << value.as_float3();

    case AttributeType::Float4:
      return stream << value.as_float4();

    case AttributeType::Bool:
      return stream << value.as_bool();

    case AttributeType::Path:
      return stream << value.as_path();

    case AttributeType::Color:
      return stream << value.as_color().as_rgba();

    case AttributeType::Object:
      return stream << "object '" << value.as_object() << "'";

    default:
      throw TactileError {"Invalid attribute type!"};
  }
}

}  // namespace tactile