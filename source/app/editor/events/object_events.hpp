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

#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

/**
 * \brief Emitted after an object has been moved.
 */
struct move_object_event final
{
  object_id id{};  ///< ID of object to move.
  float old_x{};   ///< Previous x-coordinate of the object.
  float old_y{};   ///< Previous y-coordinaete of the object.
  float new_x{};   ///< Updated x-coordinate of the object.
  float new_y{};   ///< Updated y-coordinate of the object.
};

/**
 * \brief Emitted when the visibility of an object should be changed.
 */
struct set_object_visibility_event final
{
  object_id id{};  ///< ID of target object.
  bool visible{};  ///< New visibility state.
};

/**
 * \brief Emitted when the tag of an object should be changed.
 */
struct set_object_tag_event final
{
  object_id id{};   ///< ID of target object.
  std::string tag;  ///< The new object tag.
};

/**
 * \brief Emitted when the name of an object should be changed.
 */
struct set_object_name_event final
{
  object_id id{};    ///< ID of target object.
  std::string name;  ///< The new object name.
};

struct duplicate_object_event final
{
  object_id id{};  ///< ID of the target object.
};

struct remove_object_event final
{
  object_id id{};  ///< ID of the target object.
};

struct spawn_object_context_menu_event final
{
  entt::entity object_entity{entt::null};
};

/// \} End of group events

}  // namespace tactile