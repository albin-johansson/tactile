#include "map.hpp"

#include <gtest/gtest.h>

#include "algorithm.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

using namespace tactile;

TEST(Map, Defaults)
{
  const core::Map map;
  EXPECT_EQ(0, map.LayerCount());
  EXPECT_FALSE(map.ActiveLayerId());
}

TEST(Map, DimensionConstructor)
{
  {  // Invalid dimensions
    EXPECT_THROW(core::Map(0_row, 1_col), TactileError);
    EXPECT_THROW(core::Map(1_row, 0_col), TactileError);
    EXPECT_THROW(core::Map(0_row, 0_col), TactileError);
    EXPECT_THROW(core::Map(-1_row, -1_col), TactileError);
  }

  {  // Valid dimensions
    EXPECT_NO_THROW((core::Map{1_row, 1_col}));

    const auto rows = 7_row;
    const auto cols = 5_col;
    const core::Map map{rows, cols};
    EXPECT_EQ(rows, map.RowCount());
    EXPECT_EQ(cols, map.ColumnCount());
  }
}

TEST(Map, EachLayer)
{
  core::Map map;

  map.AddTileLayer();
  map.AddTileLayer();
  map.AddTileLayer();

  int count{};
  map.EachLayer([&](const layer_id, const shared<core::ILayer>&) { ++count; });

  EXPECT_EQ(3, count);
}

TEST(Map, RemoveOccurrences)
{
  core::Map map{4_row, 4_col};

  const auto id = map.NextLayerId();
  auto layer = map.MakeTileLayer();

  std::vector<core::Position> positions;
  layer->Flood({}, 1_t, positions);

  map.AddLayer(id, layer);
  map.SelectLayer(id);
  map.RemoveOccurrences(1_t);

  layer->Each([](const tile_id tile) { EXPECT_EQ(empty, tile); });
}

TEST(Map, AddTileLayer)
{
  core::Map map{5_row, 5_col};
  const auto count = map.LayerCount();

  const auto id = map.AddTileLayer();
  EXPECT_NE(id, map.ActiveLayerId());  // new layer isn't active

  EXPECT_EQ(count + 1, map.LayerCount());
}

TEST(Map, RemoveLayer)
{
  core::Map map;

  const auto fst = map.AddTileLayer();
  const auto snd = map.AddTileLayer();
  EXPECT_EQ(2, map.LayerCount());

  EXPECT_NO_THROW(map.RemoveLayer(snd));
  EXPECT_EQ(1, map.LayerCount());

  map.SelectLayer(fst);
  map.RemoveLayer(fst);
  EXPECT_EQ(0, map.LayerCount());
  EXPECT_FALSE(map.ActiveLayerId());
}

TEST(Map, SelectLayer)
{
  core::Map map{4_row, 4_col};
  EXPECT_EQ(1_layer, map.ActiveLayerId());

  EXPECT_NO_THROW(map.SelectLayer(-1_layer));
  EXPECT_EQ(1_layer, map.ActiveLayerId());

  map.AddTileLayer();
  map.SelectLayer(1_layer);
  EXPECT_EQ(1_layer, map.ActiveLayerId());

  EXPECT_NO_THROW(map.SelectLayer(2_layer));
  EXPECT_EQ(2_layer, map.ActiveLayerId());
}

TEST(Map, AddRow)
{
  const auto row = 4_row;
  core::Map map{row, 4_col};

  map.AddRow(empty);
  EXPECT_EQ(row + 1_row, map.RowCount());
}

TEST(Map, AddCol)
{
  const auto col = 7_col;
  core::Map map{1_row, col};

  map.AddColumn(empty);
  EXPECT_EQ(col + 1_col, map.ColumnCount());
}

TEST(Map, RemoveRow)
{
  const auto initialRows = 4_row;
  core::Map map{initialRows, 10_col};

  map.RemoveRow();
  EXPECT_EQ(initialRows - 1_row, map.RowCount());

  InvokeN(20, [&map] { map.RemoveRow(); });

  EXPECT_EQ(1_row, map.RowCount());
}

TEST(Map, RemoveCol)
{
  const auto initialCols = 9_col;
  core::Map map{7_row, initialCols};

  map.RemoveColumn();
  EXPECT_EQ(initialCols - 1_col, map.ColumnCount());

  InvokeN(20, [&map] { map.RemoveColumn(); });

  EXPECT_EQ(1_col, map.ColumnCount());
}

TEST(Map, IncreaseTileSize)
{
  core::Map map;

  const auto before = map.CurrentTileSize();
  map.IncreaseTileSize();

  const auto after = map.CurrentTileSize();
  EXPECT_GT(after, before);
}

TEST(Map, DecreaseTileSize)
{
  core::Map map;

  const auto before = map.CurrentTileSize();
  map.DecreaseTileSize();

  const auto after = map.CurrentTileSize();
  EXPECT_LT(after, before);
}

TEST(Map, ResetTileSize)
{
  core::Map map;

  for (int i = 0; i < 10; ++i)
  {
    map.IncreaseTileSize();
  }

  map.ResetTileSize();
  EXPECT_EQ(core::TileSize::DefaultSize(), map.CurrentTileSize());
}

TEST(Map, SetNextLayerID)
{
  core::Map map;
  map.SetNextLayerId(7_layer);
  EXPECT_EQ(7_layer, map.NextLayerId());
}

TEST(Map, SetRowCount)
{
  core::Map map{3_row, 3_col};

  const auto nRows = 12_row;
  map.SetRowCount(nRows);

  EXPECT_EQ(3_col, map.ColumnCount());
  EXPECT_EQ(nRows, map.RowCount());

  map.SetRowCount(0_row);
  EXPECT_EQ(1_row, map.RowCount());

  map.SetRowCount(-1_row);
  EXPECT_EQ(1_row, map.RowCount());
}

TEST(Map, SetColCount)
{
  core::Map map{3_row, 3_col};

  const auto nCols = 9_col;
  map.SetColumnCount(nCols);

  EXPECT_EQ(3_row, map.RowCount());
  EXPECT_EQ(nCols, map.ColumnCount());

  map.SetColumnCount(0_col);
  EXPECT_EQ(1_col, map.ColumnCount());

  map.SetColumnCount(-1_col);
  EXPECT_EQ(1_col, map.ColumnCount());
}

TEST(Map, SetVisibility)
{
  core::Map map{5_row, 5_col};

  EXPECT_FALSE(map.IsVisible(-1_layer));  // invalid index
  EXPECT_FALSE(map.IsVisible(0_layer));   // invalid index
  EXPECT_TRUE(map.IsVisible(1_layer));    // valid index, should be true

  map.SetVisibility(1_layer, false);
  EXPECT_FALSE(map.IsVisible(1_layer));

  map.SetVisibility(1_layer, true);
  EXPECT_TRUE(map.IsVisible(1_layer));
}

TEST(Map, IsVisible)
{
  const core::Map map{5_row, 5_col};

  EXPECT_FALSE(map.IsVisible(-1_layer));
  EXPECT_FALSE(map.IsVisible(0_layer));
  EXPECT_TRUE(map.IsVisible(1_layer));
}

TEST(Map, LayerCount)
{
  core::Map map{5_row, 5_col};
  EXPECT_EQ(1, map.LayerCount());

  map.AddTileLayer();
  EXPECT_EQ(2, map.LayerCount());
}

TEST(Map, HasLayer)
{
  core::Map map{5_row, 5_col};

  EXPECT_TRUE(map.HasLayer(1_layer));
  EXPECT_FALSE(map.HasLayer(-1_layer));
  EXPECT_FALSE(map.HasLayer(2_layer));

  map.AddTileLayer();
  EXPECT_TRUE(map.HasLayer(2_layer));
  EXPECT_FALSE(map.HasLayer(3_layer));
}

TEST(Map, InBounds)
{
  const core::Map map{4_row, 4_col};

  EXPECT_TRUE(map.InBounds({0_row, 0_col}));
  EXPECT_TRUE(map.InBounds({3_row, 3_col}));
  EXPECT_FALSE(map.InBounds({4_row, 3_col}));
  EXPECT_FALSE(map.InBounds({3_row, 4_col}));
  EXPECT_FALSE(map.InBounds({4_row, 4_col}));
}

TEST(Map, RowCount)
{
  const core::Map map{7_row, 3_col};
  EXPECT_EQ(7_row, map.RowCount());
}

TEST(Map, ColCount)
{
  const core::Map map{2_row, 12_col};
  EXPECT_EQ(12_col, map.ColumnCount());
}

TEST(Map, Width)
{
  const core::Map map;
  EXPECT_EQ(map.ColumnCount().get() * map.CurrentTileSize(), map.Width());
}

TEST(Map, Height)
{
  const core::Map map;
  EXPECT_EQ(map.RowCount().get() * map.CurrentTileSize(), map.Height());
}

TEST(Map, ActiveLayerID)
{
  core::Map map{5_row, 5_col};
  EXPECT_EQ(1_layer, map.ActiveLayerId());

  map.AddTileLayer();

  map.SelectLayer(2_layer);
  EXPECT_EQ(2_layer, map.ActiveLayerId());
}

TEST(Map, CurrentTileSize)
{
  const core::Map map;
  EXPECT_EQ(core::TileSize::DefaultSize(), map.CurrentTileSize());
}

TEST(Map, GetLayer)
{
  core::Map map;
  EXPECT_ANY_THROW(map.GetLayer(1_layer));

  const auto id = map.AddTileLayer();
  EXPECT_NO_THROW(map.GetLayer(id));
}
