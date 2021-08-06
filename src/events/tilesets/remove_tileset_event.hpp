#pragma once

#include "aliases/tileset_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct RemoveTilesetEvent final
{
  tileset_id id;
};

/// \} End of group events

}  // namespace Tactile
