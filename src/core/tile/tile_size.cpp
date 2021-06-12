#include "tile_size.hpp"

#include <cmath>  // round

#include "algorithm.hpp"

namespace tactile::core {

void TileSize::Reset() noexcept
{
  mSize = DefaultSize();
}

void TileSize::Increase()
{
  mSize = static_cast<int>(std::round(static_cast<double>(mSize) * 1.1));
}

void TileSize::Decrease() noexcept
{
  const auto size = static_cast<int>(static_cast<double>(mSize) * 0.9);
  mSize = AtLeast(size, MinimumSize());
}

void TileSize::SetSize(const int size) noexcept
{
  mSize = AtLeast(size, MinimumSize());
}

}  // namespace tactile::core
