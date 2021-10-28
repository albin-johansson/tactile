#pragma once

#include <string>  // string

#include <tactile-base/object_type.hpp>
#include <tactile-base/tactile_std.hpp>

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
}  // namespace Tactile
