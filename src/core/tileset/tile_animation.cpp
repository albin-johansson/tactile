#include "tile_animation.hpp"

namespace Tactile {

void TileAnimation::Update()
{
  const auto now = cen::counter::ticks();
  if (now - mLastUpdateTime >= GetCurrentDuration())
  {
    mLastUpdateTime = now;
    mIndex = (mIndex + 1) % mFrames.size();
  }
}

void TileAnimation::AddFrame(const Frame frame)
{
  mFrames.push_back(frame);
}

auto TileAnimation::GetCurrentTile() const -> tile_id
{
  return mFrames.at(mIndex).tile;
}

auto TileAnimation::GetCurrentDuration() const -> cen::milliseconds<uint32>
{
  return mFrames.at(mIndex).duration;
}

}  // namespace Tactile
