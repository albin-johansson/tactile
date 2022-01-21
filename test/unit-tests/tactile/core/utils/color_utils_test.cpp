#include "core/utils/color_utils.hpp"

#include <gtest/gtest.h>

using namespace Tactile;

TEST(ColorUtils, ColorToArray)
{
  const auto color = cen::colors::tan;
  const auto array = ColorToArray(color);
  ASSERT_EQ(color.norm_red(), array.at(0));
  ASSERT_EQ(color.norm_green(), array.at(1));
  ASSERT_EQ(color.norm_blue(), array.at(2));
  ASSERT_EQ(color.norm_alpha(), array.at(3));
}