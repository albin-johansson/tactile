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

#pragma once

#include <pugixml.hpp>

#include "common/type/result.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

using XmlDocument = pugi::xml_document;
using XmlNode = pugi::xml_node;
using XmlAttr = pugi::xml_attribute;

/// Parses the XML document at the specified path, returning nothing if an error occurs.
[[nodiscard]] auto parse_xml_file(const Path& path) -> Maybe<XmlDocument>;

auto save_xml_to_file(const XmlDocument& document, const Path& path) -> Result;

[[nodiscard]] auto has_attr(XmlNode node, const char* attr_name) -> bool;

[[nodiscard]] auto get_string_attr(XmlNode node, const char* attr_name) -> Maybe<String>;

[[nodiscard]] auto get_int_attr(XmlNode node, const char* attr_name) -> Maybe<int32>;

[[nodiscard]] auto get_uint_attr(XmlNode node, const char* attr_name) -> Maybe<uint32>;

[[nodiscard]] auto get_float_attr(XmlNode node, const char* attr_name) -> Maybe<float>;

[[nodiscard]] auto get_bool_attr(XmlNode node, const char* attr_name) -> Maybe<bool>;

}  // namespace tactile
