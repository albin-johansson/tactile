#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "core/object_type.hpp"
#include "tactile.hpp"

namespace tactile::comp {

/**
 * \brief Component that represents objects located in object layers.
 *
 * \see active_object
 * \see object_drag_info
 */
struct object final
{
  object_id id{};      ///< Unique object identifier.
  float x{};           ///< X-coordinate.
  float y{};           ///< Y-coordinate.
  float width{};       ///< Object width (might be zero for points, etc.).
  float height{};      ///< Object height (might be zero for points, etc.).
  object_type type{};  ///< Specific object type.
  std::string tag;     ///< Optional user-provided type.
  bool visible{};      ///< Whether or not the object is rendered.
};

/**
 * \brief Context component pointing to the currently active object entity.
 */
struct active_object final
{
  entt::entity entity{entt::null};  ///< The currently active object (possibly null).
};

/**
 * \brief Component that provides temporary information about dragged objects.
 */
struct object_drag_info final
{
  float origin_object_x{};  ///< Object X-coordinate at the drag start.
  float origin_object_y{};  ///< Object Y-coordinate at the drag start.
  float last_mouse_x{};     ///< Mouse viewport x-coordinate at last update.
  float last_mouse_y{};     ///< Mouse viewport y-coordinate at last update.
};

}  // namespace tactile::comp
