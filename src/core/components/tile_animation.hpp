#pragma once

#include <centurion.hpp>  // milliseconds
#include <entt.hpp>       // entity
#include <vector>         // vector

#include "aliases/ints.hpp"
#include "aliases/tile_id.hpp"

namespace Tactile {

struct TileAnimationFrame final
{
  tile_id tile;
  cen::milliseconds<uint32> duration{};
};

struct TileAnimation final
{
  usize index{};
  cen::milliseconds<uint32> last_update_time{};
  std::vector<entt::entity> frames;
};

}  // namespace Tactile