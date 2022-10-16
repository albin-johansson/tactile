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

#include "json.hpp"

#include <iomanip>  // setw

#include "core/util/filesystem.hpp"
#include "core/util/string.hpp"
#include "io/file.hpp"
#include "io/proto/preferences.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

template <typename T>
[[nodiscard]] auto as(const JSON& json, StringView name) -> Maybe<T>
{
  const auto iter = json.find(to_std_view(name));
  if (iter != json.end()) {
    return iter->get<T>();
  }
  else {
    return nothing;
  }
}

template <>
[[nodiscard]] auto as(const JSON& json, StringView name) -> Maybe<String>
{
  const auto iter = json.find(to_std_view(name));
  if (iter != json.end()) {
    return from_std(iter->get<std::string>());
  }
  else {
    return nothing;
  }
}

}  // namespace

void to_json(JSON& json, const Attribute& value)
{
  switch (value.type()) {
    case AttributeType::String:
      json = to_std(value.as_string());
      break;

    case AttributeType::Int:
      json = value.as_int();
      break;

    case AttributeType::Float:
      json = value.as_float();
      break;

    case AttributeType::Bool:
      json = value.as_bool();
      break;

    case AttributeType::Path:
      json = to_std(convert_to_forward_slashes(value.as_path()));
      break;

    case AttributeType::Color:
      json = value.as_color().as_argb();
      break;

    case AttributeType::Object:
      json = value.as_object();
      break;

    default:
      throw TactileError("Invalid attribute type!");
  }
}

void write_json(const JSON& json, const Path& path)
{
  auto stream = write_file(path, FileType::Text);

  if (io::get_preferences().indent_output) {
    stream << std::setw(2);
  }

  stream << json;
}

auto try_get(const JSON& json, const char* key) -> const JSON*
{
  if (const auto iter = json.find(key); iter != json.end()) {
    return &iter.value();
  }
  else {
    return nullptr;
  }
}

auto read_json(const Path& path) -> Maybe<JSON>
{
  try {
    auto stream = read_file(path, FileType::Text);

    JSON json;
    stream >> json;

    return json;
  }
  catch (...) {
    return nothing;
  }
}

namespace io {

auto as_string(const JSON& json, StringView name) -> Maybe<String>
{
  return as<String>(json, name);
}

auto as_int(const JSON& json, StringView name) -> Maybe<int32>
{
  return as<int32>(json, name);
}

auto as_uint(const JSON& json, StringView name) -> Maybe<uint32>
{
  return as<uint32>(json, name);
}

auto as_float(const JSON& json, StringView name) -> Maybe<float>
{
  return as<float>(json, name);
}

auto as_bool(const JSON& json, StringView name) -> Maybe<bool>
{
  return as<bool>(json, name);
}

}  // namespace io
}  // namespace tactile
