#pragma once

#include "aliases/ints.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct UndoEvent final
{};

struct RedoEvent final
{};

struct ChangeCommandCapacityEvent final
{
  usize capacity{};
};

/// \} End of group events

}  // namespace Tactile
