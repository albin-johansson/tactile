#pragma once

#include "aliases/tileset_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SelectTilesetEvent final
{
  tileset_id id;
};

/// \} End of group events

}  // namespace Tactile
