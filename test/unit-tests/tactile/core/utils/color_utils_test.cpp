#include "core/utils/color_utils.hpp"

#include <gtest/gtest.h>

using namespace Tactile;

TEST(ColorUtils, ColorToArray)
{
  const auto color = cen::colors::tan;
  const auto array = ColorToArray(color);
  ASSERT_EQ(color.GetRedNorm(), array.at(0));
  ASSERT_EQ(color.GetGreenNorm(), array.at(1));
  ASSERT_EQ(color.GetBlueNorm(), array.at(2));
  ASSERT_EQ(color.GetAlphaNorm(), array.at(3));
}