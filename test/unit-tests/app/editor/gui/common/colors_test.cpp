#include "editor/gui/common/colors.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(ColorUtils, ColorToArray)
{
  const auto color = cen::colors::tan;
  const auto array = color_to_array(color);
  ASSERT_EQ(color.norm_red(), array.at(0));
  ASSERT_EQ(color.norm_green(), array.at(1));
  ASSERT_EQ(color.norm_blue(), array.at(2));
  ASSERT_EQ(color.norm_alpha(), array.at(3));
}