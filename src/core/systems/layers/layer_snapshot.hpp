#pragma once

#include <vector>  // vector

#include "aliases/layer_id.hpp"
#include "aliases/maybe.hpp"
#include "aliases/tile_matrix.hpp"
#include "core/components/layer.hpp"
#include "core/systems/property_context_snapshot.hpp"
#include "object_snapshot.hpp"

namespace Tactile::Sys {

struct LayerSnapshot final
{
  Layer core;
  PropertyContextSnapshot context;
  Maybe<LayerID> parent;
  Maybe<TileMatrix> tiles;
  Maybe<std::vector<ObjectSnapshot>> objects;
  Maybe<std::vector<LayerSnapshot>> children;
};

}  // namespace Tactile::Sys
