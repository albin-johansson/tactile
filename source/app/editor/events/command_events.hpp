#pragma once

#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct undo_event final
{};

struct redo_event final
{};

struct set_command_capacity_event final
{
  usize capacity{};
};

/// \} End of group events

}  // namespace tactile
