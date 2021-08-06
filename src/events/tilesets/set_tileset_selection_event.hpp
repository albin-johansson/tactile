#pragma once

#include "core/tileset/tileset_selection.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SetTilesetSelectionEvent final
{
  TilesetSelection selection;
};

/// \} End of group events

}  // namespace Tactile
