// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "json.hpp"

#include <exception>  // exception
#include <iomanip>    // setw

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/util/filesystem.hpp"
#include "io/stream.hpp"
#include "model/settings.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/io/save/vec_serialization.hpp"

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
    case AttributeType::kStr:
    case AttributeType::kInt2:
    case AttributeType::kInt3:
    case AttributeType::kInt4:
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4:
      // We store vector properties as strings, instead of just ignoring them.
      json = kStringAttrName;
      break;

    case AttributeType::kInt: json = kIntAttrName; break;

    case AttributeType::kFloat: json = kFloatAttrName; break;

    case AttributeType::kBool: json = kBoolAttrName; break;

    case AttributeType::kPath: json = kFileAttrName; break;

    case AttributeType::kColor: json = kColorAttrName; break;

    case AttributeType::kObject: json = kObjectAttrName; break;
  }
}

void to_json(JSON& json, const Attribute& value)
{
  switch (value.get_type()) {
    case AttributeType::kStr: json = value.as_string(); break;

    case AttributeType::kInt: json = value.as_int(); break;

    case AttributeType::kInt2: json = serialize(value.as_int2()); break;

    case AttributeType::kInt3: json = serialize(value.as_int3()); break;

    case AttributeType::kInt4: json = serialize(value.as_int4()); break;

    case AttributeType::kFloat: json = value.as_float(); break;

    case AttributeType::kFloat2: json = serialize(value.as_float2()); break;

    case AttributeType::kFloat3: json = serialize(value.as_float3()); break;

    case AttributeType::kFloat4: json = serialize(value.as_float4()); break;

    case AttributeType::kBool: json = value.as_bool(); break;

    case AttributeType::kPath: json = use_forward_slashes(value.as_path()); break;

    case AttributeType::kColor: json = value.as_color().to_string_argb(); break;

    case AttributeType::kObject: json = value.as_object(); break;

    default: throw Exception {"Invalid attribute type"};
  }
}

void from_json(const JSON& json, AttributeType& type)
{
  if (json == kStringAttrName.data()) {
    type = AttributeType::kStr;
  }
  else if (json == kIntAttrName.data()) {
    type = AttributeType::kInt;
  }
  else if (json == kFloatAttrName.data()) {
    type = AttributeType::kFloat;
  }
  else if (json == kBoolAttrName.data()) {
    type = AttributeType::kBool;
  }
  else if (json == kColorAttrName.data()) {
    type = AttributeType::kColor;
  }
  else if (json == kFileAttrName.data()) {
    type = AttributeType::kPath;
  }
  else if (json == kObjectAttrName.data()) {
    type = AttributeType::kObject;
  }
  else {
    throw Exception {"Not a valid JSON property type"};
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

auto save_json_to_file(const JSON& json, const Path& path) -> Result<void>
{
  auto stream = open_output_stream(path, FileType::Text);
  if (!stream) {
    spdlog::error("Could not open JSON file for writing: {}", path);
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  try {
    if (get_settings().test_flag(SETTINGS_INDENT_OUTPUT_BIT)) {
      *stream << std::setw(2);
    }

    *stream << json;
    return kOK;
  }
  catch (const std::exception& e) {
    spdlog::error("Could not save JSON: {}", e.what());
    return unexpected(make_error(GenericError::kInvalidFile));
  }
  catch (...) {
    spdlog::error("Unknown error when saving JSON to {}", path);
    return unexpected(make_error(GenericError::kUnknown));
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
