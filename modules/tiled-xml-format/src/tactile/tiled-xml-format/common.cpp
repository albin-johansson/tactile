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
inline constexpr auto kMaxI64 = std::numeric_limits<int64>::max();
inline constexpr auto kMaxU64 = std::numeric_limits<uint64>::max();
inline constexpr auto kMaxF64 = std::numeric_limits<float64>::max();

}  // namespace

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

auto parse_string(const pugi::xml_node node, const char* key) -> Result<String>
{
  if (const auto* value = node.attribute(key).as_string(nullptr)) {
    return value;
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_int64(const pugi::xml_node node, const char* key) -> Result<int64>
{
  const auto attribute = node.attribute(key);

  if (!attribute.empty()) {
    const auto value = static_cast<int64>(attribute.as_llong(kMaxI64));
    if (value != kMaxI64) {
      return value;
    }
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_uint64(const pugi::xml_node node, const char* key) -> Result<uint64>
{
  const auto attribute = node.attribute(key);

  if (!attribute.empty()) {
    const auto value = static_cast<uint64>(attribute.as_ullong(kMaxU64));
    if (value != kMaxU64) {
      return value;
    }
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

auto parse_float64(pugi::xml_node node, const char* key) -> Result<float64>
{
  const auto attribute = node.attribute(key);

  if (!attribute.empty()) {
    const auto value = static_cast<float64>(attribute.as_double(kMaxF64));
    if (value != kMaxF64) {
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

auto parse_to(pugi::xml_node node, const char* key, bool& value) -> Result<void>
{
  return parse_bool(node, key).and_then([&](const bool b) {
    value = b;
    return kOK;
  });
}

}  // namespace tactile::tiled::tmx
