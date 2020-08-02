#include "tile_size.hpp"

#include <cmath>

namespace tactile {

TileSize::TileSize() noexcept : m_size{defaultSize}
{}

void TileSize::reset() noexcept
{
  m_size = defaultSize;
}

void TileSize::increase() noexcept
{
  m_size = static_cast<int>(std::round(static_cast<double>(m_size) * 1.1));
}

void TileSize::decrease() noexcept
{
  m_size = static_cast<int>(static_cast<double>(m_size) * 0.9);
  m_size = (m_size < minimumSize) ? minimumSize : m_size;
}

void TileSize::set_size(int size) noexcept
{
  size = (size < minimumSize) ? minimumSize : size;
  m_size = size;
}

}  // namespace tactile
