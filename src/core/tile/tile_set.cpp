#include "tile_set.h"

namespace tactile {

int TileSet::get_tile_amount() const noexcept
{
  return 0;
}

int TileSet::width() const noexcept
{
  return 0;
}

int TileSet::height() const noexcept
{
  return 0;
}

TileSet::Cell::Cell(int x, int y, int index) noexcept
    : m_x{x}, m_y{y}, m_index{index}
{}

}  // namespace tactile
