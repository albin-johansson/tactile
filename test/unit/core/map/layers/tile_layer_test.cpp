#include "core/map/layers/tile_layer.hpp"

#include <gtest/gtest.h>

#include "core/tactile_error.hpp"

using namespace Tactile;

TEST(TileLayer, Defaults)
{
  const TileLayer layer{5_row, 4_col};
  ASSERT_EQ(5_row, layer.GetRowCount());
  ASSERT_EQ(4_col, layer.GetColumnCount());
  ASSERT_EQ(20, layer.GetTileCount());
  ASSERT_EQ(1.0f, layer.GetOpacity());
  ASSERT_EQ("Tile layer", layer.GetName());
  ASSERT_EQ(LayerType::TileLayer, layer.GetType());
  ASSERT_TRUE(layer.IsVisible());
}

TEST(TileLayer, Construction)
{
  ASSERT_THROW(TileLayer(0_row, 0_col), TactileError);
  ASSERT_THROW(TileLayer(1_row, 0_col), TactileError);
  ASSERT_THROW(TileLayer(0_row, 1_col), TactileError);
  ASSERT_THROW(TileLayer(-1_row, -1_col), TactileError);

  ASSERT_NO_THROW(TileLayer(1_row, 1_col));

  const auto rows = 18_row;
  const auto cols = 52_col;
  const TileLayer layer{rows, cols};
  ASSERT_EQ(rows, layer.GetRowCount());
  ASSERT_EQ(cols, layer.GetColumnCount());
}

TEST(TileLayer, Each)
{
  const TileLayer layer{4_row, 4_col};

  int count = 0;
  layer.Each([&](const tile_id id) { ++count; });

  ASSERT_EQ(count, layer.GetTileCount());
}

TEST(TileLayer, RemoveAllSingle)
{
  TileLayer layer{5_row, 5_col};

  std::vector<MapPosition> positions;
  layer.Flood({}, 1_tile, positions);
  layer.Each([](const tile_id id) { ASSERT_EQ(1_tile, id); });

  layer.RemoveAll(1_tile);
  layer.Each([](const tile_id id) { ASSERT_EQ(empty_tile, id); });
}

TEST(TileLayer, RemoveAllRange)
{
  TileLayer layer{5_row, 5_col};

  std::vector<MapPosition> positions;
  layer.Flood({}, 10_tile, positions);
  layer.SetTile({2_row, 2_col}, 42_tile);

  layer.RemoveAll(1_tile, 50_tile);
  layer.Each([](const tile_id id) { ASSERT_EQ(empty_tile, id); });
}

TEST(TileLayer, AddRow)
{
  const auto nRows = 5_row;
  const auto id = 84_tile;

  TileLayer layer{nRows, 3_col};
  layer.AddRow(id);

  ASSERT_EQ(nRows + 1_row, layer.GetRowCount());

  for (auto col = 0_col; col < layer.GetColumnCount(); ++col)
  {
    ASSERT_EQ(empty_tile, layer.GetTile({nRows - 1_row, col}));
    ASSERT_EQ(id, layer.GetTile({nRows, col}));
  }
}

TEST(TileLayer, AddColumn)
{
  const auto nCols = 7_col;
  const auto id = 33_tile;

  TileLayer layer{5_row, nCols};
  layer.AddColumn(id);

  ASSERT_EQ(nCols + 1_col, layer.GetColumnCount());

  for (auto row = 0_row; row < layer.GetRowCount(); ++row)
  {
    ASSERT_EQ(empty_tile, layer.GetTile({row, nCols - 1_col}));
    ASSERT_EQ(id, layer.GetTile({row, nCols}));
  }
}

TEST(TileLayer, RemoveRow)
{
  TileLayer layer{5_row, 5_col};

  for (int i = 0; i < 10; ++i)
  {
    ASSERT_NO_THROW(layer.RemoveRow());
  }

  ASSERT_EQ(1_row, layer.GetRowCount());
}

TEST(TileLayer, RemoveColumn)
{
  TileLayer layer{5_row, 5_col};

  for (int i = 0; i < 10; ++i)
  {
    ASSERT_NO_THROW(layer.RemoveColumn());
  }

  ASSERT_EQ(1_col, layer.GetColumnCount());
}

TEST(TileLayer, SetRowCount)
{
  TileLayer layer{5_row, 5_col};
  ASSERT_EQ(5_row, layer.GetRowCount());
  ASSERT_EQ(5_col, layer.GetColumnCount());

  layer.SetRowCount(32_row);
  ASSERT_EQ(32_row, layer.GetRowCount());
  ASSERT_EQ(5_col, layer.GetColumnCount());

  layer.SetRowCount(15_row);
  ASSERT_EQ(15_row, layer.GetRowCount());
  ASSERT_EQ(5_col, layer.GetColumnCount());
}

TEST(TileLayer, SetColumnCount)
{
  TileLayer layer{5_row, 5_col};
  ASSERT_EQ(5_row, layer.GetRowCount());
  ASSERT_EQ(5_col, layer.GetColumnCount());

  layer.SetColumnCount(41_col);
  ASSERT_EQ(5_row, layer.GetRowCount());
  ASSERT_EQ(41_col, layer.GetColumnCount());

  layer.SetColumnCount(8_col);
  ASSERT_EQ(5_row, layer.GetRowCount());
  ASSERT_EQ(8_col, layer.GetColumnCount());
}

TEST(TileLayer, SetTile)
{
  TileLayer layer{5_row, 5_col};

  const MapPosition pos{2_row, 2_col};
  ASSERT_EQ(empty_tile, layer.GetTile(pos));

  layer.SetTile(pos, 24_tile);
  ASSERT_EQ(24_tile, layer.GetTile(pos));

  ASSERT_NO_THROW(layer.SetTile({-1_row, -1_col}, 5_tile));
  ASSERT_NO_THROW(
      layer.SetTile({layer.GetRowCount(), layer.GetColumnCount()}, 7_tile));

  ASSERT_FALSE(layer.GetTile({layer.GetRowCount(), layer.GetColumnCount()}));
}

TEST(TileLayer, GetRowCount)
{
  const TileLayer layer{7_row, 12_col};
  ASSERT_EQ(7_row, layer.GetRowCount());
}

TEST(TileLayer, GetColumnCount)
{
  const TileLayer layer{7_row, 12_col};
  ASSERT_EQ(12_col, layer.GetColumnCount());
}

TEST(TileLayer, GetTileCount)
{
  const TileLayer layer{4_row, 5_col};
  ASSERT_EQ(20, layer.GetTileCount());
}

TEST(TileLayer, GetTile)
{
  const TileLayer layer;
  ASSERT_TRUE(layer.GetTile({0_row, 0_col}));
  ASSERT_TRUE(layer.GetTile({4_row, 4_col}));
  ASSERT_FALSE(layer.GetTile({4_row, 5_col}));
  ASSERT_FALSE(layer.GetTile({5_row, 4_col}));
  ASSERT_FALSE(layer.GetTile({5_row, 5_col}));
}

TEST(TileLayer, InBounds)
{
  const TileLayer layer;
  ASSERT_TRUE(layer.InBounds({0_row, 0_col}));
  ASSERT_TRUE(layer.InBounds({4_row, 4_col}));
  ASSERT_FALSE(layer.InBounds({4_row, 5_col}));
  ASSERT_FALSE(layer.InBounds({5_row, 4_col}));
  ASSERT_FALSE(layer.InBounds({5_row, 5_col}));
}
