#pragma once

#include <vector>  // vector

#include <tactile_def.hpp>

#include <entt.hpp>  // registry

namespace Tactile {

struct FancyTile final
{
  TileID id;  ///< The associated tile that has additional meta-information.
  std::vector<entt::entity> objects;  ///< Associated object entities.
};

}  // namespace Tactile
