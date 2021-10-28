#pragma once

#include <vector>  // vector

#include <tactile-base/tactile_std.hpp>

#include <centurion.hpp>  // milliseconds
#include <entt.hpp>       // entity

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