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

#include "model/event/tileset_events.hpp"
#include "model/model.hpp"

namespace tactile {

void on_show_new_tileset_dialog(Model& model, const ShowNewTilesetDialogEvent& event);

void on_create_tileset(Model& model, const CreateTilesetEvent& event);

void on_detach_tileset(Model& model, const DetachTilesetEvent& event);

}  // namespace tactile
