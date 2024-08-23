// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/render/hexagon_info.hpp"

#include <numbers>  // sqrt3_v

#include "tactile/core/debug/assert.hpp"

namespace tactile::ui {
inline namespace hexagon_info {

inline constexpr float kHalfSqrt3 = std::numbers::sqrt3_v<float> * 0.5f;

}  // namespace hexagon_info

auto get_hexagon_info(const float side_length) noexcept -> HexagonInfo
{
  const auto apothem = side_length * kHalfSqrt3;
  return {
    .side_length = side_length,
    .apothem = apothem,
    .short_diagonal = apothem * 2.0f,
    .long_diagonal = side_length * 2.0f,
  };
}

void get_hexagon_vertices(const Float2& center_pos,
                          const HexagonInfo& hexagon_info,
                          const Span<Float2> hexagon_vertices)
{
  TACTILE_ASSERT(hexagon_vertices.size() >= 6);

  const auto center_x = center_pos.x();
  const auto center_y = center_pos.y();

  const auto x_step = hexagon_info.short_diagonal * 0.5f;
  const auto y_step_short = hexagon_info.side_length * 0.5f;
  const auto y_step_long = hexagon_info.long_diagonal * 0.5f;

  // Top
  hexagon_vertices[0].set_x(center_x);
  hexagon_vertices[0].set_y(center_y - y_step_long);

  // Top-right
  hexagon_vertices[1].set_x(center_x + x_step);
  hexagon_vertices[1].set_y(center_y - y_step_short);

  // Bottom-right
  hexagon_vertices[2].set_x(center_x + x_step);
  hexagon_vertices[2].set_y(center_y + y_step_short);

  // Bottom
  hexagon_vertices[3].set_x(center_x);
  hexagon_vertices[3].set_y(center_y + y_step_long);

  // Bottom-left
  hexagon_vertices[4].set_x(center_x - x_step);
  hexagon_vertices[4].set_y(center_y + y_step_short);

  // Top-left
  hexagon_vertices[5].set_x(center_x - x_step);
  hexagon_vertices[5].set_y(center_y - y_step_short);
}

auto get_hexagon_bounding_box(const Float2& center_pos,
                              const HexagonInfo& hexagon_info) noexcept -> Float4
{
  Float4 box {};

  box[0] = center_pos.x() - hexagon_info.apothem;
  box[1] = center_pos.y() - hexagon_info.long_diagonal * 0.5f;
  box[2] = hexagon_info.apothem * 2.0f;
  box[3] = hexagon_info.long_diagonal;

  return box;
}

auto get_hexagon_center(const MatrixIndex& index,
                        const HexagonInfo& hexagon_info) noexcept -> Float2
{
  const auto is_row_odd = (index.row % 2) != 0;
  const auto odd_row_x_offset = is_row_odd ? hexagon_info.apothem : 0.0f;

  const Float2 origin {
    static_cast<float>(index.col) * hexagon_info.width() + odd_row_x_offset,
    static_cast<float>(index.row) * hexagon_info.side_length * 1.5f,
  };

  const Float2 hexagon_size {hexagon_info.width(), hexagon_info.height()};

  return origin + hexagon_size * 0.5f;
}

}  // namespace tactile::ui
