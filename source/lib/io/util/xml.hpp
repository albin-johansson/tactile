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

#pragma once

#include <pugixml.hpp>

#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "core/vocabulary.hpp"

namespace tactile::io {

using XMLNode = pugi::xml_node;

[[nodiscard]] auto has_attr(XMLNode node, const char* attr_name) -> bool;

[[nodiscard]] auto as_string(XMLNode node, const char* attr_name) -> Maybe<String>;

[[nodiscard]] auto as_int(XMLNode node, const char* attr_name) -> Maybe<int32>;

[[nodiscard]] auto as_uint(XMLNode node, const char* attr_name) -> Maybe<uint32>;

[[nodiscard]] auto as_float(XMLNode node, const char* attr_name) -> Maybe<float>;

[[nodiscard]] auto as_bool(XMLNode node, const char* attr_name) -> Maybe<bool>;

}  // namespace tactile::io
