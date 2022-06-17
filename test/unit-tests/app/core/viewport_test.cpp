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

#include "core/viewport.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using namespace core;

// TODO zoom_in, zoom_out

TEST(Viewport, Defaults)
{
  const Viewport viewport;
  ASSERT_FALSE(viewport.get_limits().has_value());

  ASSERT_EQ(0, viewport.get_offset().x);
  ASSERT_EQ(0, viewport.get_offset().y);

  ASSERT_EQ(32, viewport.get_cell_size().x);
  ASSERT_EQ(32, viewport.get_cell_size().y);
}

TEST(Viewport, ResetLimits)
{
  Viewport viewport;

  viewport.set_limits({.min_offset = {23, 12}, .max_offset = {83, 123}});
  ASSERT_TRUE(viewport.get_limits().has_value());

  viewport.reset_limits();
  ASSERT_FALSE(viewport.get_limits().has_value());
}

TEST(Viewport, Offset)
{
  Viewport viewport;

  viewport.offset({42, -12});
  ASSERT_EQ(42, viewport.get_offset().x);
  ASSERT_EQ(-12, viewport.get_offset().y);
}

TEST(Viewport, PanLeft)
{
  Viewport viewport;

  viewport.pan_left();
  ASSERT_EQ(viewport.get_cell_size().x, viewport.get_offset().x);
  ASSERT_EQ(0, viewport.get_offset().y);
}

TEST(Viewport, PanRight)
{
  Viewport viewport;

  viewport.pan_right();
  ASSERT_EQ(-viewport.get_cell_size().x, viewport.get_offset().x);
  ASSERT_EQ(0, viewport.get_offset().y);
}

TEST(Viewport, PanUp)
{
  Viewport viewport;

  viewport.pan_up();
  ASSERT_EQ(0, viewport.get_offset().x);
  ASSERT_EQ(viewport.get_cell_size().y, viewport.get_offset().y);
}

TEST(Viewport, PanDown)
{
  Viewport viewport;

  viewport.pan_down();
  ASSERT_EQ(0, viewport.get_offset().x);
  ASSERT_EQ(-viewport.get_cell_size().y, viewport.get_offset().y);
}

TEST(Viewport, PanningInOppositeDirectionsShouldCancel)
{
  Viewport viewport;

  viewport.pan_left();
  viewport.pan_right();
  ASSERT_EQ(0, viewport.get_offset().x);
  ASSERT_EQ(0, viewport.get_offset().y);

  viewport.pan_up();
  viewport.pan_down();
  ASSERT_EQ(0, viewport.get_offset().x);
  ASSERT_EQ(0, viewport.get_offset().y);
}

TEST(Viewport, SetCellSize)
{
  Viewport viewport;

  viewport.set_cell_size({93, 24});
  ASSERT_EQ(93, viewport.get_cell_size().x);
  ASSERT_EQ(24, viewport.get_cell_size().y);
}

TEST(Viewport, SetLimits)
{
  const float minX = -32;
  const float minY = -13;
  const float maxX = 483;
  const float maxY = 923;

  Viewport viewport;
  viewport.set_limits({.min_offset = {minX, minY}, .max_offset = {maxX, maxY}});

  viewport.offset({minX - 1, 0});
  ASSERT_EQ(minX, viewport.get_offset().x);
  ASSERT_EQ(0, viewport.get_offset().y);

  viewport.offset({0, minY - 1});
  ASSERT_EQ(minX, viewport.get_offset().x);
  ASSERT_EQ(minY, viewport.get_offset().y);

  viewport.offset({maxX + 100, 0});
  ASSERT_EQ(maxX, viewport.get_offset().x);
  ASSERT_EQ(minY, viewport.get_offset().y);

  viewport.offset({0, maxY + 100});
  ASSERT_EQ(maxX, viewport.get_offset().x);
  ASSERT_EQ(maxY, viewport.get_offset().y);
}
