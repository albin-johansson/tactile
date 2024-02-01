// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/tile_format.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using namespace tactile::core;

TEST(TileFormat, Defaults)
{
  const TileFormat tile_format {};
  EXPECT_EQ(tile_format.encoding, TileEncoding::kPlainText);
  EXPECT_EQ(tile_format.compression, CompressionMode::kNone);
  EXPECT_FALSE(tile_format.compression_level.has_value());
}
