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

#include <yaml-cpp/yaml.h>

#include "core/common/fs.hpp"
#include "io/fwd.hpp"
#include "io/map/parse/parse_error.hpp"
#include "io/map/parse/parse_result.hpp"

namespace tactile::io {

[[nodiscard]] auto parse_yaml_map(const fs::path& path) -> ParseResult;

[[nodiscard]] auto parse_tilesets(const YAML::Node& sequence,
                                  ir::MapData&      data,
                                  const fs::path&   dir) -> ParseError;

[[nodiscard]] auto parse_layers(const YAML::Node& sequence, ir::MapData& data)
    -> ParseError;

[[nodiscard]] auto parse_object(const YAML::Node&  node,
                                const ir::MapData& map,
                                ir::ObjectData&    object) -> ParseError;

[[nodiscard]] auto parse_component_definitions(const YAML::Node& node, ir::MapData& data)
    -> ParseError;

[[nodiscard]] auto parse_properties(const YAML::Node& node, ir::ContextData& context)
    -> ParseError;

[[nodiscard]] auto parse_components(const YAML::Node&  node,
                                    const ir::MapData& map,
                                    ir::ContextData&   context) -> ParseError;

}  // namespace tactile::io
