/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <centurion/fwd.hpp>

#include "core/common/ints.hpp"
#include "core/common/math.hpp"

namespace tactile {

/**
 * \brief An interface for renderers used by some core aspects, such as the tools.
 */
class IRenderer
{
 public:
  virtual ~IRenderer() noexcept = default;

  virtual void render_image(uint            texture,
                            const Vector2f& pos,
                            const Vector2f& size,
                            const Vector2f& uvMin,
                            const Vector2f& uvMax,
                            uint8           opacity = 0xFF) = 0;

  virtual void draw_ellipse(const Vector2f&   center,
                            const Vector2f&   radius,
                            const cen::color& color,
                            float             thickness = 1.0f) = 0;

  virtual void draw_rect(const Vector2f&   pos,
                         const Vector2f&   size,
                         const cen::color& color,
                         float             thickness = 1.0f) = 0;

  [[nodiscard]] virtual auto get_origin() const -> Vector2f = 0;

  [[nodiscard]] virtual auto get_grid_size() const -> Vector2f = 0;
};

}  // namespace tactile