#include "tile_layer.hpp"

#include <gtest/gtest.h>

#include "map_position.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

using namespace tactile;

TEST(TileLayer, Defaults)
{
  core::TileLayer layer;
  EXPECT_EQ(5_row, layer.RowCount());
  EXPECT_EQ(5_col, layer.ColumnCount());
  EXPECT_EQ(25, layer.TileCount());
  EXPECT_EQ(1.0, layer.Opacity());
  EXPECT_EQ(TACTILE_QSTRING(u"Tile layer"), layer.Name());
  EXPECT_TRUE(layer.IsVisible());
}

TEST(TileLayer, DimensionsConstructor)
{
  {  // Invalid dimensions
    EXPECT_THROW(core::TileLayer(0_row, 0_col), TactileError);
    EXPECT_THROW(core::TileLayer(1_row, 0_col), TactileError);
    EXPECT_THROW(core::TileLayer(0_row, 1_col), TactileError);
    EXPECT_THROW(core::TileLayer(-1_row, -1_col), TactileError);
  }

  {  // Valid dimensions
    EXPECT_NO_THROW(core::TileLayer(1_row, 1_col));

    constexpr auto rows = 18_row;
    constexpr auto cols = 52_col;
    const core::TileLayer layer{rows, cols};
    EXPECT_EQ(rows, layer.RowCount());
    EXPECT_EQ(cols, layer.ColumnCount());
  }
}

TEST(TileLayer, Each)
{
  core::TileLayer layer;
  auto count = 0;

  layer.Each([&](const tile_id) { ++count; });

  EXPECT_EQ(count, layer.TileCount());
}

TEST(TileLayer, RemoveAll)
{
  core::TileLayer layer;
  std::vector<core::MapPosition> positions;

  layer.Flood({}, 1_t, positions);
  layer.Each([](const tile_id id) { EXPECT_EQ(1_t, id); });

  layer.RemoveAll(1_t);
  layer.Each([](const tile_id id) { EXPECT_EQ(empty, id); });
}

TEST(TileLayer, AddRow)
{
  const auto nRows = 5_row;
  const auto id = 84_t;

  core::TileLayer layer{nRows, 3_col};
  layer.AddRow(id);

  EXPECT_EQ(nRows + 1_row, layer.RowCount());

  for (auto col = 0_col; col < layer.ColumnCount(); ++col)
  {
    EXPECT_EQ(empty, layer.TileAt({nRows - 1_row, col}));
    EXPECT_EQ(id, layer.TileAt({nRows, col}));
  }
}

TEST(TileLayer, AddCol)
{
  const auto nCols = 7_col;
  const auto id = 33_t;

  core::TileLayer layer{5_row, nCols};
  layer.AddColumn(id);

  EXPECT_EQ(nCols + 1_col, layer.ColumnCount());

  for (auto row = 0_row; row < layer.RowCount(); ++row)
  {
    EXPECT_EQ(empty, layer.TileAt({row, nCols - 1_col}));
    EXPECT_EQ(id, layer.TileAt({row, nCols}));
  }
}

TEST(TileLayer, RemoveRow)
{
  core::TileLayer layer{5_row, 5_col};

  for (int i = 0; i < 10; ++i)
  {
    EXPECT_NO_THROW(layer.RemoveRow());
  }

  EXPECT_EQ(1_row, layer.RowCount());
}

TEST(TileLayer, RemoveCol)
{
  core::TileLayer layer{5_row, 5_col};

  for (int i = 0; i < 10; ++i)
  {
    EXPECT_NO_THROW(layer.RemoveColumn());
  }

  EXPECT_EQ(1_col, layer.ColumnCount());
}

TEST(TileLayer, SetRows)
{
  core::TileLayer layer;
  EXPECT_EQ(5_row, layer.RowCount());
  EXPECT_EQ(5_col, layer.ColumnCount());

  layer.SetRows(32_row);
  EXPECT_EQ(32_row, layer.RowCount());
  EXPECT_EQ(5_col, layer.ColumnCount());

  layer.SetRows(15_row);
  EXPECT_EQ(15_row, layer.RowCount());
  EXPECT_EQ(5_col, layer.ColumnCount());
}

TEST(TileLayer, SetCols)
{
  core::TileLayer layer;
  EXPECT_EQ(5_row, layer.RowCount());
  EXPECT_EQ(5_col, layer.ColumnCount());

  layer.SetColumns(41_col);
  EXPECT_EQ(5_row, layer.RowCount());
  EXPECT_EQ(41_col, layer.ColumnCount());

  layer.SetColumns(8_col);
  EXPECT_EQ(5_row, layer.RowCount());
  EXPECT_EQ(8_col, layer.ColumnCount());
}

TEST(TileLayer, SetTile)
{
  core::TileLayer layer;

  const core::MapPosition pos{2_row, 2_col};
  EXPECT_EQ(empty, layer.TileAt(pos));

  layer.SetTile(pos, 24_t);
  EXPECT_EQ(24_t, layer.TileAt(pos));

  EXPECT_NO_THROW(layer.SetTile({-1_row, -1_col}, 5_t));
  EXPECT_NO_THROW(layer.SetTile({layer.RowCount(), layer.ColumnCount()}, 7_t));

  EXPECT_FALSE(layer.TileAt({layer.RowCount(), layer.ColumnCount()}));
}

TEST(TileLayer, SetOpacity)
{
  core::TileLayer layer;
  EXPECT_EQ(1.0, layer.Opacity());

  {  // Valid opacity
    layer.SetOpacity(0.75);
    EXPECT_EQ(0.75, layer.Opacity());
  }

  {  // Underflow opacity
    layer.SetOpacity(-1);
    EXPECT_EQ(0, layer.Opacity());
  }

  {  // Overflow opacity
    layer.SetOpacity(1.1);
    EXPECT_EQ(1, layer.Opacity());
  }
}

TEST(TileLayer, SetVisible)
{
  core::TileLayer layer;

  EXPECT_TRUE(layer.IsVisible());

  layer.SetVisible(false);
  EXPECT_FALSE(layer.IsVisible());

  layer.SetVisible(true);
  EXPECT_TRUE(layer.IsVisible());
}

TEST(TileLayer, RowCount)
{
  const core::TileLayer layer{7_row, 12_col};
  EXPECT_EQ(7_row, layer.RowCount());
}

TEST(TileLayer, ColCount)
{
  const core::TileLayer layer{7_row, 12_col};
  EXPECT_EQ(12_col, layer.ColumnCount());
}

TEST(TileLayer, TileCount)
{
  const core::TileLayer layer{4_row, 5_col};
  EXPECT_EQ(20, layer.TileCount());
}

TEST(TileLayer, TileAt)
{
  const core::TileLayer layer;
  EXPECT_TRUE(layer.TileAt({0_row, 0_col}));
  EXPECT_TRUE(layer.TileAt({4_row, 4_col}));
  EXPECT_FALSE(layer.TileAt({4_row, 5_col}));
  EXPECT_FALSE(layer.TileAt({5_row, 4_col}));
  EXPECT_FALSE(layer.TileAt({5_row, 5_col}));
}

TEST(TileLayer, InBounds)
{
  const core::TileLayer layer;
  EXPECT_TRUE(layer.InBounds({0_row, 0_col}));
  EXPECT_TRUE(layer.InBounds({4_row, 4_col}));
  EXPECT_FALSE(layer.InBounds({4_row, 5_col}));
  EXPECT_FALSE(layer.InBounds({5_row, 4_col}));
  EXPECT_FALSE(layer.InBounds({5_row, 5_col}));
}
