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

#include <exception>  // exception
#include <iomanip>    // setw

#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/util/filesystem.hpp"
#include "io/stream.hpp"
#include "model/settings.hpp"

namespace tactile {
namespace {

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
      json = "string";
      break;

    case AttributeType::Int:
      json = "int";
      break;

    case AttributeType::Float:
      json = "float";
      break;

    case AttributeType::Bool:
      json = "bool";
      break;

    case AttributeType::Path:
      json = "file";
      break;

    case AttributeType::Color:
      json = "color";
      break;

    case AttributeType::Object:
      json = "object";
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
  if (json == "string") {
    type = AttributeType::String;
  }
  else if (json == "int") {
    type = AttributeType::Int;
  }
  else if (json == "float") {
    type = AttributeType::Float;
  }
  else if (json == "bool") {
    type = AttributeType::Bool;
  }
  else if (json == "color") {
    type = AttributeType::Color;
  }
  else if (json == "file") {
    type = AttributeType::Path;
  }
  else if (json == "object") {
    type = AttributeType::Object;
  }
  else {
    throw TactileError {"Not a valid JSON property type"};
  }
}

void write_json(const JSON& json, const Path& path)
{
  auto stream = open_output_stream(path, FileType::Text).value();

  if (get_settings().test_flag(SETTINGS_INDENT_OUTPUT_BIT)) {
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
    auto stream = open_input_stream(path, FileType::Text).value();

    JSON json;
    stream >> json;

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
