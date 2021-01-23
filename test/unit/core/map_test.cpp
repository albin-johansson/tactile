#include "map.hpp"

#include <gtest/gtest.h>

#include "algorithm.hpp"
#include "tactile_error.hpp"

using namespace tactile;

TEST(Map, Defaults)
{
  const core::map map;
  EXPECT_EQ(0, map.layer_count());
  EXPECT_FALSE(map.active_layer_id());
}

TEST(Map, DimensionConstructor)
{
  {  // Invalid dimensions
    EXPECT_THROW(core::map(0_row, 1_col), tactile_error);
    EXPECT_THROW(core::map(1_row, 0_col), tactile_error);
    EXPECT_THROW(core::map(0_row, 0_col), tactile_error);
    EXPECT_THROW(core::map(-1_row, -1_col), tactile_error);
  }

  {  // Valid dimensions
    EXPECT_NO_THROW((core::map{1_row, 1_col}));

    const auto rows = 7_row;
    const auto cols = 5_col;
    const core::map map{rows, cols};
    EXPECT_EQ(rows, map.row_count());
    EXPECT_EQ(cols, map.col_count());
  }
}

TEST(Map, EachLayer)
{
  core::map map;

  map.add_tile_layer();
  map.add_tile_layer();
  map.add_tile_layer();

  int count{};
  map.each_layer([&](const layer_id, const shared<core::layer>&) {
    ++count;
  });

  EXPECT_EQ(3, count);
}

TEST(Map, RemoveOccurrences)
{
  core::map map{4_row, 4_col};

  const auto id = map.next_layer_id();
  auto layer = map.make_tile_layer();

  std::vector<core::position> positions;
  layer->flood({}, 1_t, positions);

  map.add_layer(id, layer);
  map.select_layer(id);
  map.remove_occurrences(1_t);

  layer->for_each([](const tile_id tile) {
    EXPECT_EQ(empty, tile);
  });
}

TEST(Map, AddTileLayer)
{
  core::map map{5_row, 5_col};
  const auto count = map.layer_count();

  const auto id = map.add_tile_layer();
  EXPECT_NE(id, map.active_layer_id());  // new layer isn't active

  EXPECT_EQ(count + 1, map.layer_count());
}

TEST(Map, RemoveLayer)
{
  core::map map;

  const auto fst = map.add_tile_layer();
  const auto snd = map.add_tile_layer();
  EXPECT_EQ(2, map.layer_count());

  EXPECT_NO_THROW(map.remove_layer(snd));
  EXPECT_EQ(1, map.layer_count());

  map.select_layer(fst);
  map.remove_layer(fst);
  EXPECT_EQ(0, map.layer_count());
  EXPECT_FALSE(map.active_layer_id());
}

TEST(Map, SelectLayer)
{
  core::map map{4_row, 4_col};
  EXPECT_EQ(1_layer, map.active_layer_id());

  EXPECT_NO_THROW(map.select_layer(-1_layer));
  EXPECT_EQ(1_layer, map.active_layer_id());

  map.add_tile_layer();
  map.select_layer(1_layer);
  EXPECT_EQ(1_layer, map.active_layer_id());

  EXPECT_NO_THROW(map.select_layer(2_layer));
  EXPECT_EQ(2_layer, map.active_layer_id());
}

TEST(Map, AddRow)
{
  const auto row = 4_row;
  core::map map{row, 4_col};

  map.add_row(empty);
  EXPECT_EQ(row + 1_row, map.row_count());
}

TEST(Map, AddCol)
{
  const auto col = 7_col;
  core::map map{1_row, col};

  map.add_col(empty);
  EXPECT_EQ(col + 1_col, map.col_count());
}

TEST(Map, RemoveRow)
{
  const auto initialRows = 4_row;
  core::map map{initialRows, 10_col};

  map.remove_row();
  EXPECT_EQ(initialRows - 1_row, map.row_count());

  invoke_n(20, [&map] {
    map.remove_row();
  });

  EXPECT_EQ(1_row, map.row_count());
}

TEST(Map, RemoveCol)
{
  const auto initialCols = 9_col;
  core::map map{7_row, initialCols};

  map.remove_col();
  EXPECT_EQ(initialCols - 1_col, map.col_count());

  invoke_n(20, [&map] {
    map.remove_col();
  });

  EXPECT_EQ(1_col, map.col_count());
}

TEST(Map, IncreaseTileSize)
{
  core::map map;

  const auto before = map.current_tile_size();
  map.increase_tile_size();

  const auto after = map.current_tile_size();
  EXPECT_GT(after, before);
}

TEST(Map, DecreaseTileSize)
{
  core::map map;

  const auto before = map.current_tile_size();
  map.decrease_tile_size();

  const auto after = map.current_tile_size();
  EXPECT_LT(after, before);
}

TEST(Map, ResetTileSize)
{
  core::map map;

  for (int i = 0; i < 10; ++i) {
    map.increase_tile_size();
  }

  map.reset_tile_size();
  EXPECT_EQ(core::tile_size::default_size(), map.current_tile_size());
}

TEST(Map, SetNextLayerID)
{
  core::map map;
  map.set_next_layer_id(7_layer);
  EXPECT_EQ(7_layer, map.next_layer_id());
}

TEST(Map, SetRowCount)
{
  core::map map{3_row, 3_col};

  const auto nRows = 12_row;
  map.set_row_count(nRows);

  EXPECT_EQ(3_col, map.col_count());
  EXPECT_EQ(nRows, map.row_count());

  map.set_row_count(0_row);
  EXPECT_EQ(1_row, map.row_count());

  map.set_row_count(-1_row);
  EXPECT_EQ(1_row, map.row_count());
}

TEST(Map, SetColCount)
{
  core::map map{3_row, 3_col};

  const auto nCols = 9_col;
  map.set_col_count(nCols);

  EXPECT_EQ(3_row, map.row_count());
  EXPECT_EQ(nCols, map.col_count());

  map.set_col_count(0_col);
  EXPECT_EQ(1_col, map.col_count());

  map.set_col_count(-1_col);
  EXPECT_EQ(1_col, map.col_count());
}

TEST(Map, SetVisibility)
{
  core::map map{5_row, 5_col};

  EXPECT_FALSE(map.is_visible(-1_layer));  // invalid index
  EXPECT_FALSE(map.is_visible(0_layer));   // invalid index
  EXPECT_TRUE(map.is_visible(1_layer));    // valid index, should be true

  map.set_visibility(1_layer, false);
  EXPECT_FALSE(map.is_visible(1_layer));

  map.set_visibility(1_layer, true);
  EXPECT_TRUE(map.is_visible(1_layer));
}

TEST(Map, IsVisible)
{
  const core::map map{5_row, 5_col};

  EXPECT_FALSE(map.is_visible(-1_layer));
  EXPECT_FALSE(map.is_visible(0_layer));
  EXPECT_TRUE(map.is_visible(1_layer));
}

TEST(Map, LayerCount)
{
  core::map map{5_row, 5_col};
  EXPECT_EQ(1, map.layer_count());

  map.add_tile_layer();
  EXPECT_EQ(2, map.layer_count());
}

TEST(Map, HasLayer)
{
  core::map map{5_row, 5_col};

  EXPECT_TRUE(map.has_layer(1_layer));
  EXPECT_FALSE(map.has_layer(-1_layer));
  EXPECT_FALSE(map.has_layer(2_layer));

  map.add_tile_layer();
  EXPECT_TRUE(map.has_layer(2_layer));
  EXPECT_FALSE(map.has_layer(3_layer));
}

TEST(Map, InBounds)
{
  const core::map map{4_row, 4_col};

  EXPECT_TRUE(map.in_bounds({0_row, 0_col}));
  EXPECT_TRUE(map.in_bounds({3_row, 3_col}));
  EXPECT_FALSE(map.in_bounds({4_row, 3_col}));
  EXPECT_FALSE(map.in_bounds({3_row, 4_col}));
  EXPECT_FALSE(map.in_bounds({4_row, 4_col}));
}

TEST(Map, RowCount)
{
  const core::map map{7_row, 3_col};
  EXPECT_EQ(7_row, map.row_count());
}

TEST(Map, ColCount)
{
  const core::map map{2_row, 12_col};
  EXPECT_EQ(12_col, map.col_count());
}

TEST(Map, Width)
{
  const core::map map;
  EXPECT_EQ(map.col_count().get() * map.current_tile_size(), map.width());
}

TEST(Map, Height)
{
  const core::map map;
  EXPECT_EQ(map.row_count().get() * map.current_tile_size(), map.height());
}

TEST(Map, ActiveLayerID)
{
  core::map map{5_row, 5_col};
  EXPECT_EQ(1_layer, map.active_layer_id());

  map.add_tile_layer();

  map.select_layer(2_layer);
  EXPECT_EQ(2_layer, map.active_layer_id());
}

TEST(Map, CurrentTileSize)
{
  const core::map map;
  EXPECT_EQ(core::tile_size::default_size(), map.current_tile_size());
}

TEST(Map, GetLayer)
{
  core::map map;
  EXPECT_ANY_THROW(map.get_layer(1_layer));

  const auto id = map.add_tile_layer();
  EXPECT_NO_THROW(map.get_layer(id));
}
