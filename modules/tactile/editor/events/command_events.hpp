#pragma once

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

/// \addtogroup events
/// \{

struct UndoEvent final
{};

struct RedoEvent final
{};

struct SetCommandCapacityEvent final
{
  usize capacity{};
};

/// \} End of group events

}  // namespace Tactile
