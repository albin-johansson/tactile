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

#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "common/util/string_buffer.hpp"
#include "model/registry.hpp"

namespace tactile::ui {

struct NewTilesetDialogState final {
  Entity map_entity {kNullEntity};
  Path image_path;
  StringBuffer image_path_preview_buffer {};
  Int2 tile_size {32, 32};
  bool should_open {};
};

void push_new_tileset_dialog(const Registry& registry,
                             NewTilesetDialogState& state,
                             Dispatcher& dispatcher);

}  // namespace tactile::ui
