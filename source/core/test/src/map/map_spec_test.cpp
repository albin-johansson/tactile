// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/map_spec.hpp"

#include <gtest/gtest.h>

namespace tactile {

/// \trace tactile::is_valid [MapSpec]
TEST(MapSpec, IsValid)
{
  const auto orthogonal = MapOrientation::kOrthogonal;
  const auto hex = MapOrientation::kHexagonal;

  const MapSpec bad_empty {};
  const MapSpec bad_extent_width {orthogonal, {0, 1}, {1, 1}};
  const MapSpec bad_extent_height {orthogonal, {1, 0}, {1, 1}};
  const MapSpec bad_tile_width {orthogonal, {1, 1}, {0, 1}};
  const MapSpec bad_tile_height {orthogonal, {1, 1}, {1, 0}};
  const MapSpec good_orthogonal {orthogonal, {1, 1}, {1, 1}};
  const MapSpec good_hexagonal {hex, {1, 1}, {1, 1}};

  EXPECT_FALSE(is_valid(bad_empty));
  EXPECT_FALSE(is_valid(bad_extent_width));
  EXPECT_FALSE(is_valid(bad_extent_height));
  EXPECT_FALSE(is_valid(bad_tile_width));
  EXPECT_FALSE(is_valid(bad_tile_height));
  EXPECT_TRUE(is_valid(good_orthogonal));
  EXPECT_TRUE(is_valid(good_hexagonal));
}

}  // namespace tactile
