#include "tileset_manager.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

class TilesetManagerTest : public testing::Test
{
 public:
  [[maybe_unused]] static void SetUpTestSuite()
  {
    m_interior =
        std::make_shared<core::Tileset>(1_t, "terrain.png", 32_tw, 32_th);
    m_exterior =
        std::make_shared<core::Tileset>(1026_t, "exterior.png", 32_tw, 32_th);
  }

  [[maybe_unused]] static void TearDownTestSuite()
  {
    m_exterior.reset();
    m_interior.reset();
  }

  core::TilesetManager m_manager;

  inline static Shared<core::Tileset> m_interior;
  inline static Shared<core::Tileset> m_exterior;
};

TEST_F(TilesetManagerTest, AddWithoutID)
{
  const auto a = m_manager.Add(m_interior);
  const auto b = m_manager.Add(m_exterior);

  EXPECT_LT(a, b);
  EXPECT_EQ(2, m_manager.Count());
  EXPECT_TRUE(m_manager.Contains(a));
  EXPECT_TRUE(m_manager.Contains(b));
}

TEST_F(TilesetManagerTest, AddWithID)
{
  m_manager.Add(42_ts, m_interior);
  m_manager.Add(123_ts, m_exterior);

  EXPECT_EQ(2, m_manager.Count());
  EXPECT_TRUE(m_manager.Contains(42_ts));
  EXPECT_TRUE(m_manager.Contains(123_ts));
}

TEST_F(TilesetManagerTest, Remove)
{
  constexpr auto id = 7_ts;

  m_manager.Add(id, m_interior);
  EXPECT_EQ(1, m_manager.Count());

  EXPECT_NO_THROW(m_manager.Remove(42_ts));  // Should have no effect
  EXPECT_EQ(1, m_manager.Count());

  m_manager.Remove(id);
  EXPECT_EQ(0, m_manager.Count());
}

TEST_F(TilesetManagerTest, Clear)
{
  const auto a [[maybe_unused]] = m_manager.Add(m_interior);
  const auto b [[maybe_unused]] = m_manager.Add(m_exterior);

  EXPECT_EQ(2, m_manager.Count());

  m_manager.Clear();
  EXPECT_EQ(0, m_manager.Count());

  EXPECT_NO_THROW(m_manager.Clear());
}

TEST_F(TilesetManagerTest, Rename)
{
  const auto id = m_manager.Add(m_interior);
  m_manager.Rename(id, TACTILE_QSTRING(u"foo"));

  EXPECT_EQ(TACTILE_QSTRING(u"foo"), m_manager.At(id).Name());
}

TEST_F(TilesetManagerTest, Select)
{
  EXPECT_FALSE(m_manager.HasActiveTileset());

  const auto a = m_manager.Add(m_interior);
  EXPECT_EQ(a, m_manager.CurrentTilesetId());
  EXPECT_TRUE(m_manager.HasActiveTileset());

  const auto b = m_manager.Add(m_exterior);
  EXPECT_EQ(b, m_manager.CurrentTilesetId());
  EXPECT_TRUE(m_manager.HasActiveTileset());

  m_manager.Select(a);
  EXPECT_EQ(a, m_manager.CurrentTilesetId());
  EXPECT_TRUE(m_manager.HasActiveTileset());

  m_manager.Select(std::nullopt);
  EXPECT_FALSE(m_manager.CurrentTilesetId());
  EXPECT_FALSE(m_manager.HasActiveTileset());
}

TEST_F(TilesetManagerTest, IncrementNextTilesetID)
{
  EXPECT_EQ(1_ts, m_manager.NextTilesetId());

  m_manager.IncrementNextTilesetId();
  EXPECT_EQ(2_ts, m_manager.NextTilesetId());
}

TEST_F(TilesetManagerTest, At)
{
  const auto& c_manager = m_manager;

  EXPECT_ANY_THROW(m_manager.At(0_ts));
  EXPECT_ANY_THROW(c_manager.At(0_ts));

  const auto id = m_manager.Add(m_interior);
  EXPECT_NO_THROW(m_manager.At(id));
  EXPECT_NO_THROW(c_manager.At(id));
}
