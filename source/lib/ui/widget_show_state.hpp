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

namespace tactile {

/// Tracks visibility of widgets for the "Toggle UI" feature.
struct WidgetShowState final {
  bool prev_show_layer_dock     : 1 {};
  bool prev_show_tileset_dock   : 1 {};
  bool prev_show_property_dock  : 1 {};
  bool prev_show_log_dock       : 1 {};
  bool prev_show_component_dock : 1 {};
  bool prev_show_animation_dock : 1 {};
};

}  // namespace tactile