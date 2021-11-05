#pragma once

#include <string>  // string

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

/// \addtogroup events
/// \{

/// \brief Emitted *after* an object has been moved.
struct MoveObjectEvent final
{
  ObjectID id{};  ///< ID of object to move.
  float old_x{};  ///< Previous x-coordinate of the object.
  float old_y{};  ///< Previous y-coordinaete of the object.
  float new_x{};  ///< Updated x-coordinate of the object.
  float new_y{};  ///< Updated y-coordinate of the object.
};

};

/// \brief Emitted when the tag of an object should be changed.
struct SetObjectTagEvent final
{
  ObjectID id{};    ///< ID of target object.
  std::string tag;  ///< The new object tag.
};

/// \} End of group events

}  // namespace Tactile