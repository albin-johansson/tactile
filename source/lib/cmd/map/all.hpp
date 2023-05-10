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

#include "cmd/map/add_column.hpp"
#include "cmd/map/add_row.hpp"
#include "cmd/map/fix_map_tiles.hpp"
#include "cmd/map/remove_column.hpp"
#include "cmd/map/remove_row.hpp"
#include "cmd/map/resize_map.hpp"
#include "cmd/map/set_tile_format_compression.hpp"
#include "cmd/map/set_tile_format_encoding.hpp"
#include "cmd/map/set_zlib_compression_level.hpp"
#include "cmd/map/set_zstd_compression_level.hpp"