// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/common.hpp"

#include <cstring>  // strcmp
#include <limits>   // numeric_limits
#include <ostream>  // ostream
#include <utility>  // move

#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/io/stream.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace pugi {

// This is a little hack to make it possible to utilize the stream_to_file function.
auto operator<<(std::ostream& stream, const xml_document& document) -> std::ostream&
{
  document.save(stream, " ");
  return stream;
}

}  // namespace pugi

namespace tactile::tiled::tmx {
namespace {

// Used to detect invalid attribute values.
inline constexpr int kMaxInt = std::numeric_limits<int>::max();
inline constexpr uint kMaxUInt = std::numeric_limits<uint>::max();
inline constexpr float kMaxFloat = std::numeric_limits<float>::max();

}  // namespace

auto parse_string(const pugi::xml_node node, const char* key) -> Result<String>
{
  if (const auto* value = node.attribute(key).as_string(nullptr)) {
    return value;
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_int(const pugi::xml_node node, const char* key) -> Result<int>
{
  const auto attribute = node.attribute(key);

  if (!attribute.empty()) {
    const auto value = attribute.as_int(kMaxInt);
    if (value != kMaxInt) {
      return value;
    }
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_uint(const pugi::xml_node node, const char* key) -> Result<uint>
{
  const auto attribute = node.attribute(key);

  if (!attribute.empty()) {
    const auto value = attribute.as_uint(kMaxUInt);
    if (value != kMaxUInt) {
      return value;
    }
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_float(pugi::xml_node node, const char* key) -> Result<float>
{
  const auto attribute = node.attribute(key);

  if (!attribute.empty()) {
    const auto value = attribute.as_float(kMaxFloat);
    if (value != kMaxFloat) {
      return value;
    }
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_bool(pugi::xml_node node, const char* key) -> Result<bool>
{
  const auto attribute = node.attribute(key);

  if (!attribute.empty()) {
    if (const auto* value = attribute.as_string(nullptr)) {
      return std::strcmp(value, "true") == 0 || std::strcmp(value, "1") == 0;
    }
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_to(pugi::xml_node node, const char* key, String& value) -> Result<void>
{
  return parse_string(node, key).and_then([&](String&& str) {
    value = std::move(str);
    return kOK;
  });
}

auto parse_to(pugi::xml_node node, const char* key, int& value) -> Result<void>
{
  return parse_int(node, key).and_then([&](const int i) {
    value = i;
    return kOK;
  });
}

auto parse_to(pugi::xml_node node, const char* key, uint& value) -> Result<void>
{
  return parse_uint(node, key).and_then([&](const uint i) {
    value = i;
    return kOK;
  });
}

auto parse_to(pugi::xml_node node, const char* key, usize& value) -> Result<void>
{
  return parse_uint(node, key).and_then([&](const uint i) {
    value = static_cast<usize>(i);
    return kOK;
  });
}

auto parse_to(pugi::xml_node node, const char* key, float& value) -> Result<void>
{
  return parse_float(node, key).and_then([&](const float f) {
    value = f;
    return kOK;
  });
}

auto parse_to(pugi::xml_node node, const char* key, bool& value) -> Result<void>
{
  return parse_bool(node, key).and_then([&](const bool b) {
    value = b;
    return kOK;
  });
}

auto load_xml_file(const FilePath& path) -> Result<pugi::xml_document>
{
  pugi::xml_document document {};
  const auto parse_result =
      document.load_file(path.c_str(), pugi::parse_default | pugi::parse_trim_pcdata);

  if (parse_result.status == pugi::status_ok) {
    return document;
  }

  return unexpected(make_generic_error(GenericError::kIOError));
}

auto save_xml_file(const pugi::xml_document& document,
                   const FilePath& path,
                   const int indentation) -> Result<void>
{
  const StreamToFileOptions stream_options {
    .indentation = indentation,
    .binary_mode = false,
  };

  return stream_to_file(document, path, stream_options);
}

}  // namespace tactile::tiled::tmx
