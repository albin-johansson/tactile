// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/tileset.hpp"

#include <iterator>  // distance

#include <gtest/gtest.h>

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/misc/conversion.hpp"
#include "testutil/tileset_helpers.hpp"

using namespace tactile;

/// \tests tactile::Tileset::Tileset
/// \tests tactile::Tileset::tile_count
/// \tests tactile::Tileset::get_tile_size
/// \tests tactile::Tileset::get_texture_size
/// \tests tactile::Tileset::get_texture_id
TEST(Tileset, Constructor)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const Tileset tileset {tileset_info};

  EXPECT_EQ(tileset.tile_count(), tileset_info.tile_count);
  EXPECT_EQ(tileset.column_count(), tileset_info.column_count);
  EXPECT_EQ(tileset.get_tile_size(), tileset_info.tile_size);
  EXPECT_EQ(tileset.get_texture_size(), tileset_info.texture_size);
  EXPECT_EQ(tileset.get_texture_id(), tileset_info.texture_id);

  const Float2 f_tile_size {tileset_info.tile_size};
  const Float2 f_texture_size {tileset_info.texture_size};
  const auto uv_tile_size = tileset.get_normalized_tile_size();

  EXPECT_FLOAT_EQ(uv_tile_size.x, f_tile_size.x / f_texture_size.x);
  EXPECT_FLOAT_EQ(uv_tile_size.y, f_tile_size.y / f_texture_size.y);

  EXPECT_EQ(std::distance(tileset.begin(), tileset.end()), tileset_info.tile_count);
}

/// \tests tactile::Tileset::index_of
TEST(Tileset, IndexOf)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const Tileset tileset {tileset_info};

  const auto row_count = tileset.row_count();
  const auto col_count = tileset.column_count();

  EXPECT_FALSE(tileset.index_of(TilePos {-1, 0}).has_value());
  EXPECT_FALSE(tileset.index_of(TilePos {0, -1}).has_value());
  EXPECT_FALSE(tileset.index_of(TilePos {0, col_count}).has_value());
  EXPECT_FALSE(tileset.index_of(TilePos {row_count, 0}).has_value());

  int32 current_index = 0;
  for (int32 row = 0; row < row_count; ++row) {
    for (int32 col = 0; col < col_count; ++col) {
      EXPECT_EQ(tileset.index_of(TilePos {row, col}), TileIndex {current_index});
      ++current_index;
    }
  }
}

/// \tests tactile::Tileset::get_appearance
TEST(Tileset, GetAppearance)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  Tileset tileset {tileset_info};

  const TileIndex tile_index {40};
  auto* tile = tileset.find_tile(tile_index);
  ASSERT_NE(tile, nullptr);

  TileAnimation tile_animation {};
  tile_animation.append_frame(TileAnimationFrame {TileIndex {40}, Milliseconds::zero()});
  tile_animation.append_frame(TileAnimationFrame {TileIndex {41}, Milliseconds::zero()});
  tile_animation.append_frame(TileAnimationFrame {TileIndex {42}, Milliseconds::zero()});
  tile->set_animation(tile_animation);

  // Call multiple times to make sure the caching mechanism works correctly.
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {40});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {40});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {40});

  tileset.update();
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {41});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {41});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {41});

  tileset.update();
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {42});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {42});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {42});

  tileset.update();
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {40});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {40});
  EXPECT_EQ(tileset.get_appearance(tile_index), TileIndex {40});

  // Check a tile without an animation.
  const auto other_tile_index = tile_index + TileIndex {10};
  EXPECT_EQ(tileset.get_appearance(other_tile_index), other_tile_index);

  EXPECT_THROW((void) tileset.get_appearance(TileIndex {-1}), Exception);
}

/// \tests tactile::Tileset::find_tile
TEST(Tileset, FindTile)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const auto tile_count = narrow<TileIndex::value_type>(tileset_info.tile_count);

  Tileset tileset {tileset_info};
  const auto& const_tileset = tileset;

  const TileIndex first_index {0};
  EXPECT_NE(tileset.find_tile(first_index), nullptr);
  EXPECT_NE(const_tileset.find_tile(first_index), nullptr);

  const TileIndex last_index {tile_count - 1};
  EXPECT_NE(tileset.find_tile(last_index), nullptr);
  EXPECT_NE(const_tileset.find_tile(last_index), nullptr);

  EXPECT_EQ(tileset.find_tile(TileIndex {-1}), nullptr);
  EXPECT_EQ(const_tileset.find_tile(TileIndex {-1}), nullptr);

  EXPECT_EQ(tileset.find_tile(TileIndex {tile_count}), nullptr);
  EXPECT_EQ(const_tileset.find_tile(TileIndex {tile_count}), nullptr);
}

/// \tests tactile::Tileset::get_tile
TEST(Tileset, GetTile)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const auto tile_count = narrow<TileIndex::value_type>(tileset_info.tile_count);

  Tileset tileset {tileset_info};
  const auto& const_tileset = tileset;

  const TileIndex first_index {0};
  EXPECT_NO_THROW((void) tileset.get_tile(first_index));
  EXPECT_NO_THROW((void) const_tileset.get_tile(first_index));

  const TileIndex last_index {tile_count - 1};
  EXPECT_NO_THROW((void) tileset.get_tile(last_index));
  EXPECT_NO_THROW((void) const_tileset.get_tile(last_index));

  EXPECT_THROW((void) tileset.get_tile(TileIndex {-1}), Exception);
  EXPECT_THROW((void) const_tileset.get_tile(TileIndex {-1}), Exception);

  EXPECT_THROW((void) tileset.get_tile(TileIndex {tile_count}), Exception);
  EXPECT_THROW((void) const_tileset.get_tile(TileIndex {tile_count}), Exception);
}

/// \tests tactile::Tileset::is_valid_index
TEST(Tileset, IsValidIndex)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const auto tile_count = narrow<TileIndex::value_type>(tileset_info.tile_count);

  const Tileset tileset {tileset_info};

  EXPECT_TRUE(tileset.is_valid_index(TileIndex {0}));
  EXPECT_TRUE(tileset.is_valid_index(TileIndex {24}));
  EXPECT_TRUE(tileset.is_valid_index(TileIndex {tile_count - 1}));

  EXPECT_FALSE(tileset.is_valid_index(TileIndex {-1}));
  EXPECT_FALSE(tileset.is_valid_index(TileIndex {tile_count}));
}

/// \tests tactile::Tileset::last_tile_index
TEST(Tileset, LastTileIndex)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const auto tile_count = narrow<TileIndex::value_type>(tileset_info.tile_count);

  const Tileset tileset {tileset_info};
  EXPECT_EQ(tileset.last_tile_index(), TileIndex {tile_count - 1});
}

/// \tests tactile::Tileset::row_count
/// \tests tactile::Tileset::column_count
TEST(Tileset, Dimensions)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const Tileset tileset {tileset_info};

  EXPECT_EQ(tileset.row_count(), tileset_info.tile_count / tileset_info.column_count);
  EXPECT_EQ(tileset.column_count(), tileset_info.column_count);
}
