#pragma once

#include "aliases/map_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct CloseMapEvent final
{
  map_id id;
};

/// \} End of group events

}  // namespace Tactile
