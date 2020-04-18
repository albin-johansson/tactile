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

MapPosition MapPosition::north() const noexcept
{
  return {m_row - 1, m_col};
}

MapPosition MapPosition::east() const noexcept
{
  return {m_row, m_col + 1};
}

MapPosition MapPosition::south() const noexcept
{
  return {m_row + 1, m_col};
}

MapPosition MapPosition::west() const noexcept
{
  return {m_row, m_col - 1};
}

bool operator==(const MapPosition& lhs, const MapPosition& rhs) noexcept
{
  return lhs.row() == rhs.row() && lhs.col() == rhs.col();
}

bool operator!=(const MapPosition& lhs, const MapPosition& rhs) noexcept
{
  return !(lhs == rhs);
}

}  // namespace tactile
