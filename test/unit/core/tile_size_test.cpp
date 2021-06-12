#include "tile_size.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(TileSize, Reset)
{
  core::TileSize size;

  size.SetSize(74);
  size.Reset();

  EXPECT_EQ(core::TileSize::DefaultSize(), size.Get());
}

TEST(TileSize, Increase)
{
  core::TileSize size;

  size.SetSize(100);
  size.Increase();  // Increases by 10 percent

  EXPECT_EQ(110, size.Get());
}

TEST(TileSize, Decrease)
{
  core::TileSize size;

  size.SetSize(100);
  size.Decrease();  // Decreases by 10 percent

  EXPECT_EQ(90, size.Get());

  for (int i = 0; i < 30; ++i)
  {
    size.Decrease();
  }

  EXPECT_EQ(core::TileSize::MinimumSize(), size.Get());
}

TEST(TileSize, SetSize)
{
  core::TileSize size;

  size.SetSize(83);
  EXPECT_EQ(83, size.Get());

  size.SetSize(core::TileSize::MinimumSize() - 1);
  EXPECT_EQ(core::TileSize::MinimumSize(), size.Get());
}
