#pragma once

#include <vector>  // vector

#include <tactile-base/tactile_std.hpp>

#include "object_snapshot.hpp"
#include "tactile/core/components/layer.hpp"
#include "tactile/core/systems/property_context_snapshot.hpp"

namespace Tactile::Sys {

struct LayerSnapshot final
{
  usize index{};
  Layer core;
  PropertyContextSnapshot context;
  Maybe<LayerID> parent;
  Maybe<TileMatrix> tiles;
  Maybe<std::vector<ObjectSnapshot>> objects;
  Maybe<std::vector<LayerSnapshot>> children;
};

}  // namespace Tactile::Sys
