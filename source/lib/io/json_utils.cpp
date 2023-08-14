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

#include "json_utils.hpp"

#include <exception>  // exception
#include <iomanip>    // setw

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/util/filesystem.hpp"
#include "io/save_formats.hpp"
#include "io/stream_utils.hpp"
#include "tactile/core/common/error.hpp"

namespace tactile {
namespace {

inline constexpr StringView kStringAttrName = "string";
inline constexpr StringView kIntAttrName = "int";
inline constexpr StringView kFloatAttrName = "float";
inline constexpr StringView kBoolAttrName = "bool";
inline constexpr StringView kColorAttrName = "color";
inline constexpr StringView kObjectAttrName = "object";
inline constexpr StringView kFileAttrName = "file";

template <typename T>
[[nodiscard]] auto _as(const JSON& json, StringView name) -> Maybe<T>
{
  const auto iter = json.find(name);
  if (iter != json.end()) {
    return iter->get<T>();
  }
  else {
    return kNone;
  }
}

template <>
[[nodiscard]] auto _as(const JSON& json, StringView name) -> Maybe<String>
{
  const auto iter = json.find(name);
  if (iter != json.end()) {
    return iter->get<String>();
  }
  else {
    return kNone;
  }
}

}  // namespace

void to_json(JSON& json, const PropertyType type)
{
  switch (type) {
    case PropertyType::Str:
    case PropertyType::Int2:
    case PropertyType::Int3:
    case PropertyType::Int4:
    case PropertyType::Float2:
    case PropertyType::Float3:
    case PropertyType::Float4:
      // We store vector properties as strings, instead of just ignoring them.
      json = kStringAttrName;
      break;

    case PropertyType::Int:
      json = kIntAttrName;
      break;

    case PropertyType::Float:
      json = kFloatAttrName;
      break;

    case PropertyType::Bool:
      json = kBoolAttrName;
      break;

    case PropertyType::Path:
      json = kFileAttrName;
      break;

    case PropertyType::Color:
      json = kColorAttrName;
      break;

    case PropertyType::Object:
      json = kObjectAttrName;
      break;
  }
}

void to_json(JSON& json, const Property& value)
{
  switch (value.get_type()) {
    case PropertyType::Str:
      json = value.as_string();
      break;

    case PropertyType::Int:
      json = value.as_int();
      break;

    case PropertyType::Int2:
      json = serialize_to_save_format(value.as_int2());
      break;

    case PropertyType::Int3:
      json = serialize_to_save_format(value.as_int3());
      break;

    case PropertyType::Int4:
      json = serialize_to_save_format(value.as_int4());
      break;

    case PropertyType::Float:
      json = value.as_float();
      break;

    case PropertyType::Float2:
      json = serialize_to_save_format(value.as_float2());
      break;

    case PropertyType::Float3:
      json = serialize_to_save_format(value.as_float3());
      break;

    case PropertyType::Float4:
      json = serialize_to_save_format(value.as_float4());
      break;

    case PropertyType::Bool:
      json = value.as_bool();
      break;

    case PropertyType::Path:
      json = to_forward_slashes_path(value.as_path());
      break;

    case PropertyType::Color:
      json = to_string_argb(value.as_color());
      break;

    case PropertyType::Object:
      json = value.as_object();
      break;

    default:
      throw Error {"Invalid attribute type"};
  }
}

void from_json(const JSON& json, PropertyType& type)
{
  if (json == kStringAttrName.data()) {
    type = PropertyType::Str;
  }
  else if (json == kIntAttrName.data()) {
    type = PropertyType::Int;
  }
  else if (json == kFloatAttrName.data()) {
    type = PropertyType::Float;
  }
  else if (json == kBoolAttrName.data()) {
    type = PropertyType::Bool;
  }
  else if (json == kColorAttrName.data()) {
    type = PropertyType::Color;
  }
  else if (json == kFileAttrName.data()) {
    type = PropertyType::Path;
  }
  else if (json == kObjectAttrName.data()) {
    type = PropertyType::Object;
  }
  else {
    throw Error {"Not a valid JSON property type"};
  }
}

auto parse_json_file(const Path& path) -> Maybe<JSON>
{
  try {
    auto stream = open_input_stream(path, FileType::Text);
    if (!stream) {
      spdlog::error("[JSON] Could not parse JSON file at {}", path);
      return kNone;
    }

    JSON json;
    *stream >> json;

    return json;
  }
  catch (const std::exception& e) {
    spdlog::error("[JSON] Parse error: {}", e.what());
    return kNone;
  }
  catch (...) {
    return kNone;
  }
}

auto save_json_to_file(const JSON& json, const Path& path, const int indentation)
    -> Result
{
  auto stream = open_output_stream(path, FileType::Text);
  if (!stream) {
    spdlog::error("[JSON] Could not write to JSON file at {}", path);
    return failure;
  }

  try {
    *stream << std::setw(indentation);
    *stream << json;
    return success;
  }
  catch (const std::exception& e) {
    spdlog::error("[JSON] Could not save JSON file: {}", e.what());
    return failure;
  }
  catch (...) {
    spdlog::error("[JSON] Could not save JSON file to {}", path);
    return failure;
  }
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

auto as_string(const JSON& json, StringView name) -> Maybe<String>
{
  return _as<String>(json, name);
}

auto as_int(const JSON& json, StringView name) -> Maybe<int32>
{
  return _as<int32>(json, name);
}

auto as_uint(const JSON& json, StringView name) -> Maybe<uint32>
{
  return _as<uint32>(json, name);
}

auto as_float(const JSON& json, StringView name) -> Maybe<float>
{
  return _as<float>(json, name);
}

auto as_bool(const JSON& json, StringView name) -> Maybe<bool>
{
  return _as<bool>(json, name);
}

}  // namespace tactile
