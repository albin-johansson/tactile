#include "map_position.hpp"

namespace tactile::model {
namespace {

[[nodiscard]] constexpr auto clamp_component(int comp) noexcept -> int
{
  return (comp < 0) ? 0 : comp;
}

}  // namespace

map_position::map_position(int row, int col) noexcept
    : m_row{clamp_component(row)}, m_col{clamp_component(col)}
{}

void map_position::set_row(int row) noexcept
{
  m_row = clamp_component(row);
}

void map_position::set_col(int col) noexcept
{
  m_col = clamp_component(col);
}

auto map_position::north() const noexcept -> map_position
{
  return {m_row - 1, m_col};
}

auto map_position::east() const noexcept -> map_position
{
  return {m_row, m_col + 1};
}

auto map_position::south() const noexcept -> map_position
{
  return {m_row + 1, m_col};
}

auto map_position::west() const noexcept -> map_position
{
  return {m_row, m_col - 1};
}

auto operator==(const map_position& lhs, const map_position& rhs) noexcept
    -> bool
{
  return (lhs.row() == rhs.row()) && (lhs.col() == rhs.col());
}

auto operator!=(const map_position& lhs, const map_position& rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

}  // namespace tactile::model
