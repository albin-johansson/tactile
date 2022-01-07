#pragma once

#include <vector>  // vector

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

namespace Tactile {

/**
 * \brief Component that denotes tiles with additional meta-information.
 *
 * \details Tiles with aspects such as animations, properties or associated objects are
 * considered to be "fancy" and will feature this component.
 */
struct FancyTile final
{
  TileID id{};                        ///< The ID of the associated tile.
  std::vector<entt::entity> objects;  ///< Associated object entities.
};

}  // namespace Tactile
