#pragma once

#include "core/region.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SetTilesetSelectionEvent final
{
  Region selection;
};

/// \} End of group events

}  // namespace Tactile
