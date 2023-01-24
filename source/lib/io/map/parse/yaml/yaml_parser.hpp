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

#include <yaml-cpp/yaml.h>

#include "common/type/expected.hpp"
#include "common/type/path.hpp"
#include "common/type/vec.hpp"
#include "io/map/parse/parse_error.hpp"
#include "io/map/parse/parse_result.hpp"

namespace tactile::io {

[[nodiscard]] auto parse_yaml_map(const Path& path) -> ParseResult;

[[nodiscard]] auto parse_tilesets(const YAML::Node& sequence,
                                  const ir::MapData& map,
                                  const Path& dir)
    -> Expected<Vec<ir::TilesetData>, ParseError>;

[[nodiscard]] auto parse_layers(const YAML::Node& sequence, const ir::MapData& map)
    -> Expected<Vec<ir::LayerData>, ParseError>;

[[nodiscard]] auto parse_object(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::ObjectData, ParseError>;

[[nodiscard]] auto parse_component_definitions(const YAML::Node& node)
    -> Expected<ir::ComponentMap, ParseError>;

[[nodiscard]] auto parse_properties(const YAML::Node& node)
    -> Expected<ir::AttributeMap, ParseError>;

[[nodiscard]] auto parse_components(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::ComponentMap, ParseError>;

[[nodiscard]] auto parse_context(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::ContextData, ParseError>;

}  // namespace tactile::io
