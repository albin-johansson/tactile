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

#include "json.hpp"

#include <exception>  // exception
#include <iomanip>    // setw

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/util/filesystem.hpp"
#include "io/stream.hpp"
#include "model/settings.hpp"

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
[[nodiscard]] auto as(const JSON& json, StringView name) -> Maybe<T>
{
  const auto iter = json.find(name);
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
  const auto iter = json.find(name);
  if (iter != json.end()) {
    return iter->get<String>();
  }
  else {
    return nothing;
  }
}

}  // namespace

void to_json(JSON& json, const AttributeType type)
{
  switch (type) {
    case AttributeType::String:
    case AttributeType::Int2:
    case AttributeType::Int3:
    case AttributeType::Int4:
    case AttributeType::Float2:
    case AttributeType::Float3:
    case AttributeType::Float4:
      // We store vector properties as strings, instead of just ignoring them.
      json = kStringAttrName;
      break;

    case AttributeType::Int:
      json = kIntAttrName;
      break;

    case AttributeType::Float:
      json = kFloatAttrName;
      break;

    case AttributeType::Bool:
      json = kBoolAttrName;
      break;

    case AttributeType::Path:
      json = kFileAttrName;
      break;

    case AttributeType::Color:
      json = kColorAttrName;
      break;

    case AttributeType::Object:
      json = kObjectAttrName;
      break;
  }
}

void to_json(JSON& json, const Attribute& value)
{
  switch (value.get_type()) {
    case AttributeType::String:
      json = value.as_string();
      break;

    case AttributeType::Int:
      json = value.as_int();
      break;

    case AttributeType::Int2:
      json = serialize_to_save_format(value.as_int2());
      break;

    case AttributeType::Int3:
      json = serialize_to_save_format(value.as_int3());
      break;

    case AttributeType::Int4:
      json = serialize_to_save_format(value.as_int4());
      break;

    case AttributeType::Float:
      json = value.as_float();
      break;

    case AttributeType::Float2:
      json = serialize_to_save_format(value.as_float2());
      break;

    case AttributeType::Float3:
      json = serialize_to_save_format(value.as_float3());
      break;

    case AttributeType::Float4:
      json = serialize_to_save_format(value.as_float4());
      break;

    case AttributeType::Bool:
      json = value.as_bool();
      break;

    case AttributeType::Path:
      json = use_forward_slashes(value.as_path());
      break;

    case AttributeType::Color:
      json = value.as_color().as_argb();
      break;

    case AttributeType::Object:
      json = value.as_object();
      break;

    default:
      throw TactileError {"Invalid attribute type"};
  }
}

void from_json(const JSON& json, AttributeType& type)
{
  if (json == kStringAttrName.data()) {
    type = AttributeType::String;
  }
  else if (json == kIntAttrName.data()) {
    type = AttributeType::Int;
  }
  else if (json == kFloatAttrName.data()) {
    type = AttributeType::Float;
  }
  else if (json == kBoolAttrName.data()) {
    type = AttributeType::Bool;
  }
  else if (json == kColorAttrName.data()) {
    type = AttributeType::Color;
  }
  else if (json == kFileAttrName.data()) {
    type = AttributeType::Path;
  }
  else if (json == kObjectAttrName.data()) {
    type = AttributeType::Object;
  }
  else {
    throw TactileError {"Not a valid JSON property type"};
  }
}

auto parse_json_file(const Path& path) -> Maybe<JSON>
{
  try {
    auto stream = open_input_stream(path, FileType::Text);
    if (!stream) {
      spdlog::error("Failed to open JSON file: {}", path);
      return nothing;
    }

    JSON json;
    *stream >> json;

    return json;
  }
  catch (const std::exception& e) {
    spdlog::error("JSON parse error: {}", e.what());
    return nothing;
  }
  catch (...) {
    return nothing;
  }
}

auto has_supported_tiled_json_extension(const Path& path) -> bool
{
  const auto ext = path.extension();
  return ext == ".json" || ext == ".tmj";
}

auto save_json_to_file(const JSON& json, const Path& path) -> Result
{
  auto stream = open_output_stream(path, FileType::Text);
  if (!stream) {
    spdlog::error("Could not open JSON file for writing: {}", path);
    return failure;
  }

  try {
    if (get_settings().test_flag(SETTINGS_INDENT_OUTPUT_BIT)) {
      *stream << std::setw(2);
    }

    *stream << json;
    return success;
  }
  catch (const std::exception& e) {
    spdlog::error("Could not save JSON: {}", e.what());
    return failure;
  }
  catch (...) {
    spdlog::error("Unknown error when saving JSON to {}", path);
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

}  // namespace tactile
