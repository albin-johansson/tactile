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
#include "common/type/string.hpp"
#include "common/util/string_buffer.hpp"
#include "model/model.hpp"

namespace tactile::ui {

struct RenameLayerDialogState final {
  Entity layer {kNullEntity};
  String old_name;
  StringBuffer name_buffer {};
  bool should_open {};
};

void push_rename_layer_dialog(const Model& model,
                              RenameLayerDialogState& state,
                              Dispatcher& dispatcher);

}  // namespace tactile::ui
