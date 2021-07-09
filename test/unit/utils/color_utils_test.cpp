#include "utils/color_utils.hpp"

#include <gtest/gtest.h>

using namespace Tactile;

TEST(ColorUtils, ColorToArray)
{
  const auto color = cen::colors::honey_dew;
  const auto array = ColorToArray(color);
  ASSERT_EQ(color.red() / 255.0f, array.at(0));
  ASSERT_EQ(color.green() / 255.0f, array.at(1));
  ASSERT_EQ(color.blue() / 255.0f, array.at(2));
  ASSERT_EQ(color.alpha() / 255.0f, array.at(3));
}

TEST(ColorUtils, ColorFromStringARGB)
{
  ASSERT_FALSE(ColorFromString("1A2B3C4D"));    // Missing leading '#'
  ASSERT_FALSE(ColorFromString("#1A2B3C4DD"));  // Too long
  ASSERT_FALSE(ColorFromString("#1A2B3C4"));    // Too short

  const auto color = ColorFromString("#1A2B3C4D");
  ASSERT_TRUE(color);

  ASSERT_EQ(0x1A, color->alpha());
  ASSERT_EQ(0x2B, color->red());
  ASSERT_EQ(0x3C, color->green());
  ASSERT_EQ(0x4D, color->blue());
}

TEST(ColorUtils, ColorFromStringRGB)
{
  ASSERT_FALSE(ColorFromString("A2B4C6"));    // Missing leading '#'
  ASSERT_FALSE(ColorFromString("#A2B4C66"));  // Too long
  ASSERT_FALSE(ColorFromString("#A2B4C"));    // Too short

  const auto color = ColorFromString("#A2B4C6");
  ASSERT_TRUE(color);

  ASSERT_EQ(0xA2, color->red());
  ASSERT_EQ(0xB4, color->green());
  ASSERT_EQ(0xC6, color->blue());
  ASSERT_EQ(0xFF, color->alpha());
}
