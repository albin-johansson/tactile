// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/texture_io.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(TextureIO, LoadTexture)
{
  EXPECT_FALSE(load_texture("bad.png").has_value());
  // TODO
}
