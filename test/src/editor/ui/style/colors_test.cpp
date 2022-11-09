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

#include "editor/ui/style/colors.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(ColorUtils, ColorToArray)
{
  const auto color = cen::colors::tan;
  const auto array = ui::color_to_array(color);
  ASSERT_EQ(color.norm_red(), array.at(0));
  ASSERT_EQ(color.norm_green(), array.at(1));
  ASSERT_EQ(color.norm_blue(), array.at(2));
  ASSERT_EQ(color.norm_alpha(), array.at(3));
}

TEST(ColorUtils, Luminance)
{
  ASSERT_EQ(1.0f, ui::luminance(cen::colors::white));
  ASSERT_EQ(0.0f, ui::luminance(cen::colors::black));

  // Based on https://planetcalc.com/7778/
  ASSERT_FLOAT_EQ(0.4457104f, ui::luminance(cen::colors::lime_green));
  ASSERT_FLOAT_EQ(0.0186408f, ui::luminance(cen::colors::dark_blue));
  ASSERT_FLOAT_EQ(0.3465843f, ui::luminance(cen::colors::hot_pink));

  ASSERT_FALSE(ui::is_dark(cen::colors::white));
  ASSERT_TRUE(ui::is_dark(cen::colors::black));
}

}  // namespace tactile::test
