#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // entity, null

#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

/// \brief Emitted *after* an object has been moved.
struct MoveObjectEvent final
{
  object_id id{};  ///< ID of object to move.
  float old_x{};   ///< Previous x-coordinate of the object.
  float old_y{};   ///< Previous y-coordinaete of the object.
  float new_x{};   ///< Updated x-coordinate of the object.
  float new_y{};   ///< Updated y-coordinate of the object.
};

/// \brief Emitted when the visibility of an object should be changed.
struct SetObjectVisibilityEvent final
{
  object_id id{};  ///< ID of target object.
  bool visible{};  ///< New visibility state.
};

/// \brief Emitted when the tag of an object should be changed.
struct SetObjectTagEvent final
{
  object_id id{};   ///< ID of target object.
  std::string tag;  ///< The new object tag.
};

/// \brief Emitted when the name of an object should be changed.
struct SetObjectNameEvent final
{
  object_id id{};    ///< ID of target object.
  std::string name;  ///< The new object name.
};

struct DuplicateObjectEvent final
{
  object_id id{};  ///< ID of the target object.
};

struct RemoveObjectEvent final
{
  object_id id{};  ///< ID of the target object.
};

struct SpawnObjectContextMenuEvent final
{
  entt::entity object_entity{entt::null};
};

/// \} End of group events

}  // namespace tactile