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

#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

auto stringify(const AttributeType type) -> const char*
{
  switch (type) {
    case AttributeType::String:
      return "string";

    case AttributeType::Int:
      return "int";

    case AttributeType::Float:
      return "float";

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

auto operator<<(std::ostream& stream, const AttributeType type) -> std::ostream&
{
  return stream << stringify(type);
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

    case AttributeType::Float:
      set_value<float_type>(0);
      break;

    case AttributeType::Bool:
      set_value<bool>(false);
      break;

    case AttributeType::Path:
      set_value<path_type>(path_type {});
      break;

    case AttributeType::Color:
      set_value<color_type>(cen::colors::black);
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
  if (const auto* str = std::get_if<string_type>(&mValue)) {
    return str->empty();
  }
  else if (const auto* i = std::get_if<integer_type>(&mValue)) {
    return *i == 0;
  }
  else if (const auto* f = std::get_if<float_type>(&mValue)) {
    return *f == 0;
  }
  else if (const auto* b = std::get_if<bool>(&mValue)) {
    return !*b;
  }
  else if (const auto* path = std::get_if<path_type>(&mValue)) {
    return path->empty();
  }
  else if (const auto* obj = std::get_if<object_t>(&mValue)) {
    return *obj == object_t {};
  }
  else if (const auto* color = std::get_if<color_type>(&mValue)) {
    return *color == cen::colors::black;
  }
  else {
    throw TactileError {"Invalid property type!"};
  }
}

auto Attribute::type() const noexcept -> AttributeType
{
  if (holds<integer_type>()) {
    return AttributeType::Int;
  }
  else if (holds<float_type>()) {
    return AttributeType::Float;
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

auto Attribute::is_float() const noexcept -> bool
{
  return type() == AttributeType::Float;
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

auto Attribute::as_float() const -> float_type
{
  if (const auto* f = get_if<float_type>()) {
    return *f;
  }
  else {
    throw TactileError {"Attribute was not a float!"};
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

auto operator<<(std::ostream& stream, const Attribute& value) -> std::ostream&
{
  switch (value.type()) {
    case AttributeType::String:
      return stream << value.as_string();

    case AttributeType::Int:
      return stream << value.as_int();

    case AttributeType::Float:
      return stream << value.as_float();

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