#include "utils/color_utils.hpp"

#include <gtest/gtest.h>

using namespace Tactile;

TEST(ColorUtils, ColorToArray)
{
  const auto color = cen::colors::tan;
  const auto array = ColorToArray(color);
  ASSERT_EQ(color.red_norm(), array.at(0));
  ASSERT_EQ(color.green_norm(), array.at(1));
  ASSERT_EQ(color.blue_norm(), array.at(2));
  ASSERT_EQ(color.alpha_norm(), array.at(3));
}