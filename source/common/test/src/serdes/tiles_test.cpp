// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/serdes/tiles.hpp"

#include <array>
#include <cstdint>
#include <span>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/numeric/literals.hpp"

namespace tactile::common {
namespace {

using testing::Each;
using testing::Eq;

// tactile::common::serialize_tile_matrix
TEST(SerDesTiles, SerializeTileMatrixWithInsufficientOutputBuffer)
{
  // Requires 16 bytes
  const TileMatrix tile_matrix {
    TileRow {0x1A2A3A4A, 0x1B2B3B4B},
    TileRow {0x1C2C3C4C, 0x1D2D3D4D},
  };

  std::array<std::uint8_t, 15> insufficient_buffer {};
  std::array<std::uint8_t, 16> sufficient_buffer {};

  ASSERT_FALSE(serialize_tile_matrix(tile_matrix, insufficient_buffer).has_value());
  EXPECT_THAT(insufficient_buffer, Each(Eq(std::uint8_t {0})));

  const auto serialized_length = serialize_tile_matrix(tile_matrix, sufficient_buffer);
  ASSERT_TRUE(serialized_length.has_value());
  ASSERT_EQ(*serialized_length, 16_uz);

  auto index = 0_uz;
  EXPECT_EQ(sufficient_buffer[index++], 0x4Au);
  EXPECT_EQ(sufficient_buffer[index++], 0x3Au);
  EXPECT_EQ(sufficient_buffer[index++], 0x2Au);
  EXPECT_EQ(sufficient_buffer[index++], 0x1Au);

  EXPECT_EQ(sufficient_buffer[index++], 0x4Bu);
  EXPECT_EQ(sufficient_buffer[index++], 0x3Bu);
  EXPECT_EQ(sufficient_buffer[index++], 0x2Bu);
  EXPECT_EQ(sufficient_buffer[index++], 0x1Bu);

  EXPECT_EQ(sufficient_buffer[index++], 0x4Cu);
  EXPECT_EQ(sufficient_buffer[index++], 0x3Cu);
  EXPECT_EQ(sufficient_buffer[index++], 0x2Cu);
  EXPECT_EQ(sufficient_buffer[index++], 0x1Cu);

  EXPECT_EQ(sufficient_buffer[index++], 0x4Du);
  EXPECT_EQ(sufficient_buffer[index++], 0x3Du);
  EXPECT_EQ(sufficient_buffer[index++], 0x2Du);
  EXPECT_EQ(sufficient_buffer[index++], 0x1Du);
}

// tactile::common::deserialize_tile_matrix
TEST(SerDesTiles, DeserializeTileMatrix)
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

  constexpr Extent2D extent {.rows = 3, .cols = 2};
  const auto tile_matrix =
      deserialize_tile_matrix(byte_stream, extent, TileIdFormat::kTactile);

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

// tactile::common::deserialize_tile_matrix
TEST(SerDesTiles, DeserializeTileMatrixWithInsufficientData)
{
  const ByteStream byte_stream {
    // clang-format off
    0x11, 0x22, 0x33, 0x44, // Tile 0
    0x11, 0x22, 0x33, 0x44, // Tile 1
    0x11, 0x22, 0x33, 0x44, // Tile 3
    0x11, 0x22, 0x33, 0x44, // Tile 4
    0x11, 0x22, 0x33, 0x44, // Tile 5
    0x11, 0x22, 0x33,       // Tile 6
    // clang-format on
  };

  constexpr Extent2D extent {.rows = 3, .cols = 2};
  const auto tile_matrix =
      deserialize_tile_matrix(byte_stream, extent, TileIdFormat::kTactile);

  EXPECT_FALSE(tile_matrix.has_value());
}

// tactile::common::deserialize_tile_matrix
TEST(SerDesTiles, DeserializeTileMatrixWithTooMuchData)
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

  constexpr Extent2D extent {.rows = 3, .cols = 2};
  const auto tile_matrix =
      deserialize_tile_matrix(byte_stream, extent, TileIdFormat::kTactile);

  EXPECT_FALSE(tile_matrix.has_value());
}

// tactile::common::serialize_tile_matrix
// tactile::common::deserialize_tile_matrix
TEST(SerDesTiles, Roundtrip)
{
  const TileMatrix original_tile_matrix {
    TileRow {10, 11, 12, 13},
    TileRow {20, 21, 22, 23},
    TileRow {30, 31, 32, 33},
  };

  std::array<std::uint8_t, 64_uz> serialization_buffer {};
  const auto serialized_length =
      serialize_tile_matrix(original_tile_matrix, serialization_buffer);
  ASSERT_TRUE(serialized_length.has_value());
  EXPECT_EQ(*serialized_length, 12_uz * sizeof(TileID));

  std::span<const std::uint8_t> serialized_tile_matrix {serialization_buffer.data(),
                                                        *serialized_length};

  const auto new_tile_matrix = deserialize_tile_matrix(serialized_tile_matrix,
                                                       Extent2D {.rows = 3, .cols = 4},
                                                       TileIdFormat::kTactile);
  ASSERT_TRUE(new_tile_matrix.has_value());

  ASSERT_EQ(new_tile_matrix->size(), 3);
  ASSERT_EQ(new_tile_matrix->at(0).size(), 4);
  ASSERT_EQ(new_tile_matrix->at(1).size(), 4);
  ASSERT_EQ(new_tile_matrix->at(2).size(), 4);

  EXPECT_EQ(new_tile_matrix->at(0).at(0), TileID {10});
  EXPECT_EQ(new_tile_matrix->at(0).at(1), TileID {11});
  EXPECT_EQ(new_tile_matrix->at(0).at(2), TileID {12});
  EXPECT_EQ(new_tile_matrix->at(0).at(3), TileID {13});

  EXPECT_EQ(new_tile_matrix->at(1).at(0), TileID {20});
  EXPECT_EQ(new_tile_matrix->at(1).at(1), TileID {21});
  EXPECT_EQ(new_tile_matrix->at(1).at(2), TileID {22});
  EXPECT_EQ(new_tile_matrix->at(1).at(3), TileID {23});

  EXPECT_EQ(new_tile_matrix->at(2).at(0), TileID {30});
  EXPECT_EQ(new_tile_matrix->at(2).at(1), TileID {31});
  EXPECT_EQ(new_tile_matrix->at(2).at(2), TileID {32});
  EXPECT_EQ(new_tile_matrix->at(2).at(3), TileID {33});
}

}  // namespace
}  // namespace tactile::common
