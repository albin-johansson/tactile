// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "xml.hpp"

#include <algorithm>  // any_of
#include <cstring>    // strcmp

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "io/stream.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "ui/constants.hpp"

namespace tactile {

auto parse_xml_file(const Path& path) -> Maybe<XmlDocument>
{
  pugi::xml_document document;
  const auto result =
      document.load_file(path.c_str(), pugi::parse_default | pugi::parse_trim_pcdata);
  if (result.status == pugi::status_ok) {
    return document;
  }
  else {
    return nothing;
  }
}

auto save_xml_to_file(const XmlDocument& document, const Path& path) -> Result<void>
{
  auto stream = open_output_stream(path, FileType::Text);

  if (!stream) {
    spdlog::error("Could not open XML file for writing: {}", path);
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  document.save(*stream, " ");
  return kOK;
}

auto has_attr(XmlNode node, const char* attr_name) -> bool
{
  TACTILE_ASSERT(attr_name != nullptr);
  return std::any_of(node.attributes_begin(),
                     node.attributes_end(),
                     [attr_name](const XmlAttr attribute) {
                       return std::strcmp(attribute.name(), attr_name) == 0;
                     });
}

auto get_string_attr(XmlNode node, const char* attr_name) -> Maybe<String>
{
  TACTILE_ASSERT(attr_name);

  if (const char* str = node.attribute(attr_name).as_string(nullptr)) {
    return str;
  }
  else {
    return nothing;
  }
}

auto get_int_attr(XmlNode node, const char* attr_name) -> Maybe<int32>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_int(kMaxInt);

  if (!attribute.empty() && value != kMaxInt) {
    return value;
  }
  else {
    return nothing;
  }
}

auto get_uint_attr(XmlNode node, const char* attr_name) -> Maybe<uint32>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_uint(kMaxUint);

  if (!attribute.empty() && value != kMaxUint) {
    return value;
  }
  else {
    return nothing;
  }
}

auto get_float_attr(XmlNode node, const char* attr_name) -> Maybe<float>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_float(kMaxFloat);

  if (!attribute.empty() && value != kMaxFloat) {
    return value;
  }
  else {
    return nothing;
  }
}

auto get_bool_attr(XmlNode node, const char* attr_name) -> Maybe<bool>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  if (!attribute.empty()) {
    return attribute.as_bool();
  }
  else {
    return nothing;
  }
}

}  // namespace tactile
