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

  core::tileset_manager m_manager;

  inline static shared<core::Tileset> m_interior;
  inline static shared<core::Tileset> m_exterior;
};

TEST_F(TilesetManagerTest, AddWithoutID)
{
  const auto a = m_manager.add(m_interior);
  const auto b = m_manager.add(m_exterior);

  EXPECT_LT(a, b);
  EXPECT_EQ(2, m_manager.count());
  EXPECT_TRUE(m_manager.contains(a));
  EXPECT_TRUE(m_manager.contains(b));
}

TEST_F(TilesetManagerTest, AddWithID)
{
  m_manager.add(42_ts, m_interior);
  m_manager.add(123_ts, m_exterior);

  EXPECT_EQ(2, m_manager.count());
  EXPECT_TRUE(m_manager.contains(42_ts));
  EXPECT_TRUE(m_manager.contains(123_ts));
}

TEST_F(TilesetManagerTest, Remove)
{
  constexpr auto id = 7_ts;

  m_manager.add(id, m_interior);
  EXPECT_EQ(1, m_manager.count());

  EXPECT_NO_THROW(m_manager.remove(42_ts));  // Should have no effect
  EXPECT_EQ(1, m_manager.count());

  m_manager.remove(id);
  EXPECT_EQ(0, m_manager.count());
}

TEST_F(TilesetManagerTest, Clear)
{
  const auto a [[maybe_unused]] = m_manager.add(m_interior);
  const auto b [[maybe_unused]] = m_manager.add(m_exterior);

  EXPECT_EQ(2, m_manager.count());

  m_manager.clear();
  EXPECT_EQ(0, m_manager.count());

  EXPECT_NO_THROW(m_manager.clear());
}

TEST_F(TilesetManagerTest, Rename)
{
  const auto id = m_manager.add(m_interior);
  m_manager.rename(id, TACTILE_QSTRING(u"foo"));

  EXPECT_EQ(TACTILE_QSTRING(u"foo"), m_manager.at(id).Name());
}

TEST_F(TilesetManagerTest, Select)
{
  EXPECT_FALSE(m_manager.has_active_tileset());

  const auto a = m_manager.add(m_interior);
  EXPECT_EQ(a, m_manager.current_tileset_id());
  EXPECT_TRUE(m_manager.has_active_tileset());

  const auto b = m_manager.add(m_exterior);
  EXPECT_EQ(b, m_manager.current_tileset_id());
  EXPECT_TRUE(m_manager.has_active_tileset());

  m_manager.select(a);
  EXPECT_EQ(a, m_manager.current_tileset_id());
  EXPECT_TRUE(m_manager.has_active_tileset());

  m_manager.select(std::nullopt);
  EXPECT_FALSE(m_manager.current_tileset_id());
  EXPECT_FALSE(m_manager.has_active_tileset());
}

TEST_F(TilesetManagerTest, IncrementNextTilesetID)
{
  EXPECT_EQ(1_ts, m_manager.next_tileset_id());

  m_manager.increment_next_tileset_id();
  EXPECT_EQ(2_ts, m_manager.next_tileset_id());
}

TEST_F(TilesetManagerTest, At)
{
  const auto& c_manager = m_manager;

  EXPECT_ANY_THROW(m_manager.at(0_ts));
  EXPECT_ANY_THROW(c_manager.at(0_ts));

  const auto id = m_manager.add(m_interior);
  EXPECT_NO_THROW(m_manager.at(id));
  EXPECT_NO_THROW(c_manager.at(id));
}
