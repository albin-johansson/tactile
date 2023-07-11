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

#include "common/type/path.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/parsers/parse_error.hpp"

namespace tactile {

[[nodiscard]] auto parse_map_with_yaml_format(const Path& path) -> Parsed<MapIR>;

[[nodiscard]] auto parse_map_with_tiled_tmj_format(const Path& path) -> Parsed<MapIR>;

[[nodiscard]] auto parse_map_with_tiled_tmx_format(const Path& path) -> Parsed<MapIR>;

[[nodiscard]] auto parse_map(const Path& path) -> Parsed<MapIR>;

}  // namespace tactile
