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

#include "io/maps/parser/parse_data.hpp"
#include "tactile.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_object(const YAML::Node& node,
                                const ir::MapData& map,
                                ir::ObjectData* object) -> parse_error;

[[nodiscard]] auto parse_layers(const YAML::Node& sequence, ir::MapData& data)
    -> parse_error;

}  // namespace tactile::parsing