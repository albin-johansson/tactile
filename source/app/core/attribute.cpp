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

#include "misc/throw.hpp"

namespace tactile {

auto stringify(const AttributeType type) -> const char*
{
  switch (type) {
    case AttributeType::string:
      return "string";

    case AttributeType::integer:
      return "int";

    case AttributeType::floating:
      return "float";

    case AttributeType::boolean:
      return "bool";

    case AttributeType::file:
      return "file";

    case AttributeType::color:
      return "color";

    case AttributeType::object:
      return "object";

    default:
      panic("Invalid attribute type!");
  }
}

auto operator<<(std::ostream& stream, const AttributeType type) -> std::ostream&
{
  return stream << stringify(type);
}

void Attribute::reset_to_default(const AttributeType type)
{
  switch (type) {
    case AttributeType::string:
      set_value<string_type>(string_type{});
      break;

    case AttributeType::integer:
      set_value<integer_type>(0);
      break;

    case AttributeType::floating:
      set_value<float_type>(0);
      break;

    case AttributeType::boolean:
      set_value<bool>(false);
      break;

    case AttributeType::file:
      set_value<file_type>(file_type{});
      break;

    case AttributeType::color:
      set_value<color_type>(cen::colors::black);
      break;

    case AttributeType::object:
      set_value<object_t>(object_t{});
      break;

    default:
      panic("Invalid attribute type!");
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
  else if (const auto* path = std::get_if<file_type>(&mValue)) {
    return path->empty();
  }
  else if (const auto* obj = std::get_if<object_t>(&mValue)) {
    return *obj == object_t{};
  }
  else if (const auto* color = std::get_if<color_type>(&mValue)) {
    return *color == cen::colors::black;
  }
  else {
    panic("Invalid property type!");
  }
}

auto Attribute::as_string() const -> const string_type&
{
  if (const auto* str = get_if<string_type>()) {
    return *str;
  }
  else {
    panic("Attribute was not a string!");
  }
}

auto Attribute::as_int() const -> integer_type
{
  if (const auto* i = get_if<integer_type>()) {
    return *i;
  }
  else {
    panic("Attribute was not an integer!");
  }
}

auto Attribute::as_float() const -> float_type
{
  if (const auto* f = get_if<float_type>()) {
    return *f;
  }
  else {
    panic("Attribute was not a float!");
  }
}

auto Attribute::as_bool() const -> bool
{
  if (const auto* b = get_if<bool>()) {
    return *b;
  }
  else {
    panic("Attribute was not a boolean!");
  }
}

auto Attribute::as_file() const -> const file_type&
{
  if (const auto* file = get_if<file_type>()) {
    return *file;
  }
  else {
    panic("Attribute was not a file!");
  }
}

auto Attribute::as_object() const -> object_t
{
  if (const auto* obj = get_if<object_t>()) {
    return *obj;
  }
  else {
    panic("Attribute was not an object reference!");
  }
}

auto Attribute::as_color() const -> const color_type&
{
  if (const auto* color = get_if<color_type>()) {
    return *color;
  }
  else {
    panic("Attribute was not a color!");
  }
}

auto operator<<(std::ostream& stream, const Attribute& value) -> std::ostream&
{
  switch (value.type()) {
    case AttributeType::string:
      return stream << value.as_string();

    case AttributeType::integer:
      return stream << value.as_int();

    case AttributeType::floating:
      return stream << value.as_float();

    case AttributeType::boolean:
      return stream << value.as_bool();

    case AttributeType::file:
      return stream << value.as_file();

    case AttributeType::color:
      return stream << value.as_color().as_rgba();

    case AttributeType::object:
      return stream << "object '" << value.as_object() << "'";

    default:
      panic("Invalid attribute type!");
  }
}

}  // namespace tactile