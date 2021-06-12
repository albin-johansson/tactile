#include "tile_size.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(TileSize, Reset)
{
  core::tile_size size;

  size.set_size(74);
  size.reset();

  EXPECT_EQ(core::tile_size::default_size(), size.get());
}

TEST(TileSize, Increase)
{
  core::tile_size size;

  size.set_size(100);
  size.increase();  // Increases by 10 percent

  EXPECT_EQ(110, size.get());
}

TEST(TileSize, Decrease)
{
  core::tile_size size;

  size.set_size(100);
  size.decrease();  // Decreases by 10 percent

  EXPECT_EQ(90, size.get());

  for (int i = 0; i < 30; ++i)
  {
    size.decrease();
  }

  EXPECT_EQ(core::tile_size::min_size(), size.get());
}

TEST(TileSize, SetSize)
{
  core::tile_size size;

  size.set_size(83);
  EXPECT_EQ(83, size.get());

  size.set_size(core::tile_size::min_size() - 1);
  EXPECT_EQ(core::tile_size::min_size(), size.get());
}
