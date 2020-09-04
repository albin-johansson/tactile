#include "tile_size.hpp"

#include <cmath>

#include "algorithm.hpp"

namespace tactile::model {

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
  m_size = at_least(size, min_size());
}

void tile_size::set_size(int size) noexcept
{
  m_size = at_least(size, min_size());
}

}  // namespace tactile::model
