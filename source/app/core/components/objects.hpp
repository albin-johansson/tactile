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

#include <entt/entity/entity.hpp>

#include "core/object_type.hpp"
#include "tactile.hpp"

namespace tactile::comp {

/**
 * \brief Component that represents objects located in object layers.
 *
 * \ingroup components
 */
struct Object final
{
  ObjectID id{};      ///< Unique object identifier.
  float x{};          ///< X-coordinate.
  float y{};          ///< Y-coordinate.
  float width{};      ///< Object width (might be zero for points, etc.).
  float height{};     ///< Object height (might be zero for points, etc.).
  ObjectType type{};  ///< Specific object type.
  std::string tag;    ///< Optional user-provided type.
  bool visible{};     ///< Whether or not the object is rendered.
};

/**
 * \brief Context component used to keep track of active object.
 *
 * \ingroup components
 */
struct ActiveObject final
{
  entt::entity entity{entt::null};  ///< The currently active object (possibly null).
};

/**
 * \brief Component that provides temporary information about dragged objects.
 *
 * \ingroup components
 */
struct ObjectDragInfo final
{
  float origin_object_x{};  ///< Object X-coordinate at the drag start.
  float origin_object_y{};  ///< Object Y-coordinate at the drag start.
  float last_mouse_x{};     ///< Mouse viewport x-coordinate at last update.
  float last_mouse_y{};     ///< Mouse viewport y-coordinate at last update.
};

}  // namespace tactile::comp
