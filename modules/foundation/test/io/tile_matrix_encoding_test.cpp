// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/tile_matrix_encoding.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using namespace tactile::int_literals;

/// \tests tactile::tile_matrix_to_byte_stream
TEST(TileMatrixEncoding, TileMatrixToByteStream)
{
  const MatrixExtent extent {2, 3};

  // [ 10, 20, 30 ]
  // [ 40, 50, 60 ]
  auto tile_matrix = make_tile_matrix(extent);
  tile_matrix[0][0] = TileID {0xFACE};
  tile_matrix[0][1] = TileID {0xDEAD};
  tile_matrix[0][2] = TileID {0x1234BEEF};
  tile_matrix[1][0] = TileID {0x0012FEED};
  tile_matrix[1][1] = TileID {0xFADE};
  tile_matrix[1][2] = TileID {0xABCD};

  const auto byte_stream = tile_matrix_to_byte_stream(tile_matrix);
  ASSERT_EQ(byte_stream.size(), extent.row_count * extent.col_count * sizeof(TileID));

  // FACE
  EXPECT_EQ(byte_stream[0], 0xCE);
  EXPECT_EQ(byte_stream[1], 0xFA);
  EXPECT_EQ(byte_stream[2], 0x00);
  EXPECT_EQ(byte_stream[3], 0x00);

  // DEAD
  EXPECT_EQ(byte_stream[4], 0xAD);
  EXPECT_EQ(byte_stream[5], 0xDE);
  EXPECT_EQ(byte_stream[6], 0x00);
  EXPECT_EQ(byte_stream[7], 0x00);

  // 1234BEEF
  EXPECT_EQ(byte_stream[8], 0xEF);
  EXPECT_EQ(byte_stream[9], 0xBE);
  EXPECT_EQ(byte_stream[10], 0x34);
  EXPECT_EQ(byte_stream[11], 0x12);

  // 0012FEED
  EXPECT_EQ(byte_stream[12], 0xED);
  EXPECT_EQ(byte_stream[13], 0xFE);
  EXPECT_EQ(byte_stream[14], 0x12);
  EXPECT_EQ(byte_stream[15], 0x00);

  // FADE
  EXPECT_EQ(byte_stream[16], 0xDE);
  EXPECT_EQ(byte_stream[17], 0xFA);
  EXPECT_EQ(byte_stream[18], 0x00);
  EXPECT_EQ(byte_stream[19], 0x00);

  // ABCD
  EXPECT_EQ(byte_stream[20], 0xCD);
  EXPECT_EQ(byte_stream[21], 0xAB);
  EXPECT_EQ(byte_stream[22], 0x00);
  EXPECT_EQ(byte_stream[23], 0x00);
}

/// \tests tactile::tile_matrix_from_byte_stream
/// \todo Test Tiled tile flipping bit logic
TEST(TileMatrixEncoding, TileMatrixFromByteStream)
{
  const MatrixExtent extent {3, 4};
  const TileMatrix initial_tile_matrix = {
    {TileID {1}, TileID {2}, TileID {3}, TileID {4}},
    {TileID {5}, TileID {6}, TileID {7}, TileID {8}},
    {TileID {9}, TileID {10}, TileID {11}, TileID {12}},
  };

  const auto byte_stream = tile_matrix_to_byte_stream(initial_tile_matrix);
  const auto tile_matrix =
      tile_matrix_from_byte_stream(byte_stream, extent, TileEncodingFormat::kTactile);

  EXPECT_EQ(tile_matrix, initial_tile_matrix);
}

class TileMatrixEncodingCompressionModes
  : public testing::TestWithParam<CompressionMode> {};

INSTANTIATE_TEST_SUITE_P(ParameterizedTileMatrixEncoding,
                         TileMatrixEncodingCompressionModes,
                         testing::Values(CompressionMode::kNone,
                                         CompressionMode::kZlib,
                                         CompressionMode::kZstd));

/// \tests tactile::base64_encode_tile_matrix
/// \tests tactile::base64_decode_tile_matrix
TEST_P(TileMatrixEncodingCompressionModes, Base64EncodeAndDecodeTileMatrix)
{
  const auto compression_mode = GetParam();

  const MatrixExtent extent {3, 4};
  const TileMatrix initial_tile_matrix = {
    {TileID {0x100}, TileID {0x200}, TileID {0x300}, TileID {0x400}},
    {TileID {0x500}, TileID {0x600}, TileID {0x700}, TileID {0x800}},
    {TileID {0x900}, TileID {0xA00}, TileID {0xB00}, TileID {0xC00}},
  };

  const auto encoded_tiles =
      base64_encode_tile_matrix(initial_tile_matrix, compression_mode);
  ASSERT_TRUE(encoded_tiles.has_value());

  const auto decoded_tiles = base64_decode_tile_matrix(*encoded_tiles,
                                                       extent,
                                                       compression_mode,
                                                       TileEncodingFormat::kTactile);
  ASSERT_TRUE(decoded_tiles.has_value());
  EXPECT_EQ(decoded_tiles, initial_tile_matrix);
}

/// \tests tactile::base64_encode_tile_matrix
/// \tests tactile::base64_decode_tile_matrix
TEST_P(TileMatrixEncodingCompressionModes,
       Base64EncodeAndDecodeTileMatrixWithTileFlippingBits)
{
  const auto compression_mode = GetParam();

  const MatrixExtent extent {3, 4};

  // clang-format off
  const auto tile00 = static_cast<TileID::value_type>(0x100u | kTiledFlippedVerticallyBit);
  const auto tile03 = static_cast<TileID::value_type>(0x400u | kTiledFlippedDiagonallyBit);
  const auto tile12 = static_cast<TileID::value_type>(0x700u | kTiledTileFlippingMask);
  const auto tile21 = static_cast<TileID::value_type>(0xA00u | kTiledRotatedHexagonal120Bit);
  const auto tile23 = static_cast<TileID::value_type>(0xC00u | kTiledFlippedHorizontallyBit);
  // clang-format on

  const TileMatrix initial_tile_matrix = {
    {TileID {tile00}, TileID {0x200}, TileID {0x300}, TileID {tile03}},
    {TileID {0x500}, TileID {0x600}, TileID {tile12}, TileID {0x800}},
    {TileID {0x900}, TileID {tile21}, TileID {0xB00}, TileID {tile23}},
  };

  const TileMatrix expected_tile_matrix = {
    {TileID {0x100}, TileID {0x200}, TileID {0x300}, TileID {0x400}},
    {TileID {0x500}, TileID {0x600}, TileID {0x700}, TileID {0x800}},
    {TileID {0x900}, TileID {0xA00}, TileID {0xB00}, TileID {0xC00}},
  };

  const auto encoded_tiles =
      base64_encode_tile_matrix(initial_tile_matrix, compression_mode);
  ASSERT_TRUE(encoded_tiles.has_value());

  const auto decoded_tiles = base64_decode_tile_matrix(*encoded_tiles,
                                                       extent,
                                                       compression_mode,
                                                       TileEncodingFormat::kTiled);
  ASSERT_TRUE(decoded_tiles.has_value());
  EXPECT_EQ(decoded_tiles, expected_tile_matrix);
}
