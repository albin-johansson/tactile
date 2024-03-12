// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

using XmlDocument = pugi::xml_document;
using XmlNode = pugi::xml_node;
using XmlAttr = pugi::xml_attribute;

/// Parses the XML document at the specified path, returning nothing if an error occurs.
[[nodiscard]] auto parse_xml_file(const Path& path) -> Maybe<XmlDocument>;

auto save_xml_to_file(const XmlDocument& document, const Path& path) -> Result<void>;

[[nodiscard]] auto has_attr(XmlNode node, const char* attr_name) -> bool;

[[nodiscard]] auto get_string_attr(XmlNode node, const char* attr_name) -> Maybe<String>;

[[nodiscard]] auto get_int_attr(XmlNode node, const char* attr_name) -> Maybe<int32>;

[[nodiscard]] auto get_uint_attr(XmlNode node, const char* attr_name) -> Maybe<uint32>;

[[nodiscard]] auto get_float_attr(XmlNode node, const char* attr_name) -> Maybe<float>;

[[nodiscard]] auto get_bool_attr(XmlNode node, const char* attr_name) -> Maybe<bool>;

}  // namespace tactile
