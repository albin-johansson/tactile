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

#include "core/ecs/entity.hpp"
#include "model/model_view.hpp"

namespace tactile {

struct TilesetEditorViewportState final {
  Entity tileset_entity {kNullEntity};
  bool animation_frame_selection_mode : 1 {false};
};

void push_tileset_editor_viewport(ModelView& model,
                                  TilesetEditorViewportState& state,
                                  Entity tileset_document_entity);

}  // namespace tactile