#pragma once

#include "aliases/map_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SelectMapEvent final
{
  map_id id;
};

/// \} End of group events

}  // namespace Tactile
