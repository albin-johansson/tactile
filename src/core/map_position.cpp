#include "map_position.h"

namespace tactile {
namespace {

[[nodiscard]] int clamp_component(int comp) noexcept
{
  return (comp < 0) ? 0 : comp;
}

}  // namespace

MapPosition::MapPosition() noexcept : m_row{0}, m_col{0}
{}

MapPosition::MapPosition(int row, int col) noexcept
    : m_row{clamp_component(row)}, m_col{clamp_component(col)}
{}

void MapPosition::set_row(int row) noexcept
{
  m_row = clamp_component(row);
}

void MapPosition::set_col(int col) noexcept
{
  m_col = clamp_component(col);
}

}  // namespace tactile
