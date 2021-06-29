#pragma once

#include "core/tileset/tileset_selection.hpp"

namespace Tactile {

struct SetTilesetSelectionEvent final
{
  TilesetSelection selection;
};

}  // namespace Tactile
