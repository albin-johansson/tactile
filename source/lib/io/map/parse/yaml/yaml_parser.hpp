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

#include "io/ir/map/map_ir.hpp"
#include "io/parsers/parse_error.hpp"
#include "tactile/core/containers/vector.hpp"
#include "tactile/core/functional/expected.hpp"
#include "tactile/core/io/filesystem.hpp"

namespace tactile {

[[nodiscard]] auto parse_yaml_map(const Path& path) -> Parsed<MapIR>;

[[nodiscard]] auto parse_tilesets(const YAML::Node& sequence,
                                  const MapIR& map,
                                  const Path& dir) -> Parsed<Vector<TilesetIR>>;

[[nodiscard]] auto parse_layers(const YAML::Node& sequence, const MapIR& map)
    -> Parsed<Vector<LayerIR>>;

[[nodiscard]] auto parse_object(const YAML::Node& node, const MapIR& map)
    -> Parsed<ObjectIR>;

[[nodiscard]] auto parse_component_definitions(const YAML::Node& node)
    -> Parsed<ComponentMap>;

[[nodiscard]] auto parse_properties(const YAML::Node& node) -> Parsed<AttributeMap>;

[[nodiscard]] auto parse_components(const YAML::Node& node, const MapIR& map)
    -> Parsed<ComponentMap>;

[[nodiscard]] auto parse_context(const YAML::Node& node, const MapIR& map)
    -> Parsed<ContextIR>;

}  // namespace tactile
