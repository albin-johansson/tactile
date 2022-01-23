#include "ir_animation_frame.hpp"

#include <cassert>  // assert

#include "ir_definitions.hpp"

namespace tactile::IO {

void SetTile(AnimationFrame& frame, const int32 tile)
{
  frame.tile = tile;
}

void SetDuration(AnimationFrame& frame, const int32 ms)
{
  assert(ms >= 0);
  frame.duration = ms;
}

auto GetTile(const AnimationFrame& frame) -> int32
{
  return frame.tile;
}

auto GetDuration(const AnimationFrame& frame) -> int32
{
  return frame.duration;
}

}  // namespace tactile::IO