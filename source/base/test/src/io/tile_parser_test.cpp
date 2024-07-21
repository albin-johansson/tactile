// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/io/tile_parser.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

// tactile::parse_raw_tile_matrix
TEST(TileParser, ParseRawTileMatrix)
{
  const ByteStream byte_stream {
    // clang-format off
    0x11, 0x22, 0x33, 0x44, // Tile 0
    0x11, 0x22, 0x33, 0x44, // Tile 1
    0x11, 0x22, 0x33, 0x44, // Tile 3
    0x11, 0x22, 0x33, 0x44, // Tile 4
    0x11, 0x22, 0x33, 0x44, // Tile 5
    0x11, 0x22, 0x33, 0x44, // Tile 6
    // clang-format on
  };

  const MatrixExtent extent {3, 2};
  const auto tile_matrix = parse_raw_tile_matrix(byte_stream, extent, TileIdFormat::kTactile);

  ASSERT_TRUE(tile_matrix.has_value());

  ASSERT_EQ(std::ssize(*tile_matrix), extent.rows);
  ASSERT_EQ(std::ssize(tile_matrix->at(0)), extent.cols);
  ASSERT_EQ(std::ssize(tile_matrix->at(1)), extent.cols);
  ASSERT_EQ(std::ssize(tile_matrix->at(2)), extent.cols);

  EXPECT_EQ(tile_matrix->at(0).at(0), TileID {0x44332211});
  EXPECT_EQ(tile_matrix->at(0).at(1), TileID {0x44332211});

  EXPECT_EQ(tile_matrix->at(1).at(0), TileID {0x44332211});
  EXPECT_EQ(tile_matrix->at(1).at(1), TileID {0x44332211});

  EXPECT_EQ(tile_matrix->at(2).at(0), TileID {0x44332211});
  EXPECT_EQ(tile_matrix->at(2).at(1), TileID {0x44332211});
}

// tactile::parse_raw_tile_matrix
TEST(TileParser, ParseRawTileMatrixWithInsufficientData)
{
  const ByteStream byte_stream {
    // clang-format off
    0x11, 0x22, 0x33, 0x44, // Tile 0
    0x11, 0x22, 0x33, 0x44, // Tile 1
    0x11, 0x22, 0x33, 0x44, // Tile 3
    0x11, 0x22, 0x33, 0x44, // Tile 4
    0x11, 0x22, 0x33, 0x44, // Tile 5
    0x11, 0x22, 0x33,      // Tile 6
    // clang-format on
  };

  const MatrixExtent extent {3, 2};
  const auto tile_matrix = parse_raw_tile_matrix(byte_stream, extent, TileIdFormat::kTactile);

  EXPECT_FALSE(tile_matrix.has_value());
}

// tactile::parse_raw_tile_matrix
TEST(TileParser, ParseRawTileMatrixWithTooMuchData)
{
  const ByteStream byte_stream {
    // clang-format off
    0x11, 0x22, 0x33, 0x44, // Tile 0
    0x11, 0x22, 0x33, 0x44, // Tile 1
    0x11, 0x22, 0x33, 0x44, // Tile 3
    0x11, 0x22, 0x33, 0x44, // Tile 4
    0x11, 0x22, 0x33, 0x44, // Tile 5
    0x11, 0x22, 0x33, 0x44, // Tile 6
    0x11,
    // clang-format on
  };

  const MatrixExtent extent {3, 2};
  const auto tile_matrix = parse_raw_tile_matrix(byte_stream, extent, TileIdFormat::kTactile);

  EXPECT_FALSE(tile_matrix.has_value());
}

}  // namespace tactile::test
