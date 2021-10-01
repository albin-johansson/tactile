#pragma once

#include <vector>  // vector

#include "aliases/tile_matrix.hpp"
#include "common/layer_id.hpp"
#include "common/maybe.hpp"
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
