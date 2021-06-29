#include "core/tileset/tileset_manager.hpp"

#include <gtest/gtest.h>

#include "utils/load_texture.hpp"

using namespace tactile;

struct TilesetManagerTest : testing::Test
{
  [[maybe_unused]] static void SetUpTestSuite()
  {
    const auto interiorInfo = LoadTexture("resources/interior.png");
    ASSERT_TRUE(interiorInfo.has_value());

    const auto exteriorInfo = LoadTexture("resources/exterior.png");
    ASSERT_TRUE(exteriorInfo.has_value());

    mInteriorTex = interiorInfo->texture;
    mExteriorTex = exteriorInfo->texture;

    mInterior = std::make_shared<Tileset>(1_tile, *interiorInfo, 32, 32);
    mExterior = std::make_shared<Tileset>(1026_tile, *exteriorInfo, 32, 32);
  }

  [[maybe_unused]] static void TearDownTestSuite()
  {
    mExterior.reset();
    mInterior.reset();

    glDeleteTextures(1, &mExteriorTex);
    glDeleteTextures(1, &mInteriorTex);
  }

  TilesetManager mManager;

  inline static uint mInteriorTex;
  inline static uint mExteriorTex;
  inline static Shared<Tileset> mInterior;
  inline static Shared<Tileset> mExterior;
};

TEST_F(TilesetManagerTest, Add)
{
  ASSERT_EQ(0, mManager.GetSize());
  ASSERT_EQ(1_tset, mManager.GetNextTilesetId());
  ASSERT_EQ(1_tile, mManager.GetNextGlobalTileId());
  ASSERT_FALSE(mManager.Contains(42_tset));
  ASSERT_FALSE(mManager.Contains(64_tset));
  ASSERT_FALSE(mManager.GetActiveTilesetId());

  mManager.Add(42_tset, mInterior);
  ASSERT_EQ(42_tset, mManager.GetActiveTilesetId());
  ASSERT_EQ(1_tset, mManager.GetNextTilesetId());

  mManager.Add(64_tset, mExterior);
  ASSERT_EQ(64_tset, mManager.GetActiveTilesetId());
  ASSERT_EQ(1_tset, mManager.GetNextTilesetId());

  ASSERT_EQ(2, mManager.GetSize());
  ASSERT_TRUE(mManager.Contains(42_tset));
  ASSERT_TRUE(mManager.Contains(64_tset));

  mManager.Add(1_tset, mExterior);
  ASSERT_EQ(2_tset, mManager.GetNextTilesetId());
  ASSERT_EQ(3, mManager.GetSize());
}

TEST_F(TilesetManagerTest, AddWithoutID)
{
  ASSERT_EQ(0, mManager.GetSize());
  ASSERT_FALSE(mManager.GetActiveTilesetId());

  const auto interior = mManager.Add(mInterior);
  ASSERT_EQ(1, mManager.GetSize());
  ASSERT_EQ(interior, mManager.GetActiveTilesetId());
  ASSERT_TRUE(mManager.Contains(interior));
}

TEST_F(TilesetManagerTest, AddWithClashingID)
{
  ASSERT_EQ(1_tset, mManager.GetNextTilesetId());

  /* Add the same tileset thrice, but with different IDs */
  mManager.Add(1_tset, mInterior);
  mManager.Add(2_tset, mInterior);
  mManager.Add(3_tset, mInterior);
  ASSERT_EQ(4_tset, mManager.GetNextTilesetId());

  /* Leave a gap in the used identifiers, shouldn't affect next tileset ID */
  mManager.Add(5_tset, mInterior);
  ASSERT_EQ(4_tset, mManager.GetNextTilesetId());

  const auto exterior = mManager.Add(mExterior);
  ASSERT_EQ(4_tset, exterior);
  ASSERT_EQ(6_tset, mManager.GetNextTilesetId());
}

TEST_F(TilesetManagerTest, Remove)
{
  const auto interior = mManager.Add(mInterior);
  ASSERT_EQ(1, mManager.GetSize());
  ASSERT_TRUE(mManager.Contains(interior));

  mManager.Remove(interior);
  ASSERT_EQ(0, mManager.GetSize());
  ASSERT_FALSE(mManager.Contains(interior));
}

TEST_F(TilesetManagerTest, Clear)
{
  const auto a [[maybe_unused]] = mManager.Add(mInterior);
  const auto b [[maybe_unused]] = mManager.Add(mExterior);
  ASSERT_EQ(2, mManager.GetSize());
  ASSERT_TRUE(mManager.GetActiveTilesetId());

  mManager.Clear();
  ASSERT_EQ(0, mManager.GetSize());
  ASSERT_FALSE(mManager.GetActiveTilesetId());
}

TEST_F(TilesetManagerTest, Rename)
{
  const auto interior = mManager.Add(mInterior);

  mManager.Rename(interior, "foo");
  ASSERT_EQ("foo", mManager.GetName(interior));

  mManager.Rename(interior, "bar");
  ASSERT_EQ("bar", mManager.GetName(interior));
}

TEST_F(TilesetManagerTest, Select)
{
  ASSERT_FALSE(mManager.HasActiveTileset());

  const auto interior = mManager.Add(mInterior);
  ASSERT_EQ(interior, mManager.GetActiveTilesetId());

  mManager.Select(nothing);
  ASSERT_FALSE(mManager.GetActiveTilesetId());

  mManager.Select(interior);
  ASSERT_EQ(interior, mManager.GetActiveTilesetId());
}

TEST_F(TilesetManagerTest, IncrementNextTilesetId)
{
  ASSERT_EQ(1_tset, mManager.GetNextTilesetId());

  mManager.Add(2_tset, mInterior);
  mManager.Add(3_tset, mExterior);
  ASSERT_EQ(1_tset, mManager.GetNextTilesetId());

  mManager.IncrementNextTilesetId();
  ASSERT_EQ(4_tset, mManager.GetNextTilesetId());
}
