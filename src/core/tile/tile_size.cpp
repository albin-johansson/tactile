#include "tile_size.hpp"

#include <cmath>  // round

#include "algorithm.hpp"

namespace tactile::core {

void tile_size::reset() noexcept
{
  m_size = default_size();
}

void tile_size::increase()
{
  m_size = static_cast<int>(std::round(static_cast<double>(m_size) * 1.1));
}

void tile_size::decrease() noexcept
{
  const auto size = static_cast<int>(static_cast<double>(m_size) * 0.9);
  m_size = AtLeast(size, min_size());
}

void tile_size::set_size(const int size) noexcept
{
  m_size = AtLeast(size, min_size());
}

}  // namespace tactile::core
