#pragma once

#include "aliases/tile_id.hpp"

namespace Tactile {

struct FancyTile final
{
  tile_id id;  ///< The associated tile that has additional meta-information.
};

}  // namespace Tactile
