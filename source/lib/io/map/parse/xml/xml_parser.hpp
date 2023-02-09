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

#include "common/type/expected.hpp"
#include "common/type/path.hpp"
#include "io/ir/map_ir.hpp"
#include "io/map/parse/parse_error.hpp"
#include "io/map/parse/parse_result.hpp"
#include "io/util/xml.hpp"

namespace tactile {

[[nodiscard]] auto parse_xml_map(const Path& path) -> ParseResult;

[[nodiscard]] auto parse_tileset(XmlNode node, const Path& dir)
    -> Expected<ir::TilesetData, ParseError>;

[[nodiscard]] auto parse_object(XmlNode object_node)
    -> Expected<ir::ObjectData, ParseError>;

[[nodiscard]] auto parse_layers(XmlNode map_node, ir::MapData& map)
    -> Expected<Vec<ir::LayerData>, ParseError>;

[[nodiscard]] auto parse_properties(XmlNode node)
    -> Expected<ir::AttributeMap, ParseError>;

[[nodiscard]] auto parse_context(XmlNode node) -> Expected<ir::ContextData, ParseError>;

}  // namespace tactile