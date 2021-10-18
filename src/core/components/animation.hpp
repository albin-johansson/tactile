#pragma once

#include <vector>  // vector

#include <centurion.hpp>  // milliseconds
#include <entt.hpp>       // entity

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

struct AnimationFrame final
{
  TileID tile;
  cen::milliseconds<uint32> duration{};
};

struct Animation final
{
  usize index{};
  cen::milliseconds<uint32> last_update_time{};
  std::vector<entt::entity> frames;
};

}  // namespace Tactile