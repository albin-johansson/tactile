#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt.hpp>  // entity, null

namespace Tactile {

struct Object final
{
  ObjectID id;              ///< Unique object identifier.
  float x{};                ///< X-coordinate.
  float y{};                ///< Y-coordinate.
  float width{};            ///< Object width (might be zero for points, etc.).
  float height{};           ///< Object height (might be zero for points, etc.).
  ObjectType type{};        ///< Specific object type.
  std::string custom_type;  ///< Optional user-provided type.
  bool visible{};           ///< Whether or not the object is rendered.
};

struct ActiveObject final
{
  entt::entity entity{entt::null};
};

struct ObjectDragInfo final
{
  float origin_object_x{};  ///< Object X-coordinate at drag start.
  float origin_object_y{};  ///< Object Y-coordinate at drag start.
  float last_mouse_x{};     ///< Mouse viewport x-coordinate at last update.
  float last_mouse_y{};     ///< Mouse viewport y-coordinate at last update.
};

}  // namespace Tactile
