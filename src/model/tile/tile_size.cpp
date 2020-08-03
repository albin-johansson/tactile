#include "tile_size.hpp"

#include <cmath>

namespace tactile::model {

void tile_size::reset() noexcept
{
  m_size = default_size();
}

void tile_size::increase() noexcept
{
  m_size = static_cast<int>(std::round(static_cast<double>(m_size) * 1.1));
}

void tile_size::decrease() noexcept
{
  m_size = static_cast<int>(static_cast<double>(m_size) * 0.9);
  m_size = (m_size < min_size()) ? min_size() : m_size;
}

void tile_size::set_size(int size) noexcept
{
  size = (size < min_size()) ? min_size() : size;
  m_size = size;
}

}  // namespace tactile::model
