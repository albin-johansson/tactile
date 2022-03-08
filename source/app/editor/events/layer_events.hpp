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

#include <string>  // string

#include "core/layer_type.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct add_layer_event final
{
  layer_type type{};
};

struct remove_layer_event final
{
  layer_id id{};
};

struct duplicate_layer_event final
{
  layer_id id{};
};

struct select_layer_event final
{
  layer_id id{};
};

struct move_layer_up_event final
{
  layer_id id{};
};

struct move_layer_down_event final
{
  layer_id id{};
};

struct set_layer_opacity_event final
{
  layer_id id{};
  float opacity{};
};

struct set_layer_visible_event final
{
  layer_id id{};
  bool visible{};
};

struct open_rename_layer_dialog_event final
{
  layer_id id{};
};

struct rename_layer_event final
{
  layer_id id{};
  std::string name;
};

/// \} End of group events

}  // namespace tactile
