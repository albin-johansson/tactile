#pragma once

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

struct MoveObjectEvent final
{
  ObjectID id{};
  float old_x{};
  float old_y{};
  float new_x{};
  float new_y{};
};

}  // namespace Tactile