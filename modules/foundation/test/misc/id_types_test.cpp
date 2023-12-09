// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/id_types.hpp"

#include <gtest/gtest.h>

using namespace tactile;

/// \tests tactile::kEmptyTile
TEST(IdTypes, TileID)
{
  EXPECT_EQ(kEmptyTile.value, 0);
}
