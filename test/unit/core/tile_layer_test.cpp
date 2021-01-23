#include "tile_layer.hpp"

#include <gtest/gtest.h>

#include "algorithm.hpp"
#include "position.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

using namespace tactile;

TEST(TileLayer, Defaults)
{
  core::tile_layer layer;
  EXPECT_EQ(5_row, layer.row_count());
  EXPECT_EQ(5_col, layer.col_count());
  EXPECT_EQ(25, layer.tile_count());
  EXPECT_EQ(1.0, layer.opacity());
  EXPECT_EQ(TACTILE_QSTRING(u"Tile layer"), layer.name());
  EXPECT_TRUE(layer.visible());
}

TEST(TileLayer, DimensionsConstructor)
{
  {  // Invalid dimensions
    EXPECT_THROW(core::tile_layer(0_row, 0_col), tactile_error);
    EXPECT_THROW(core::tile_layer(1_row, 0_col), tactile_error);
    EXPECT_THROW(core::tile_layer(0_row, 1_col), tactile_error);
    EXPECT_THROW(core::tile_layer(-1_row, -1_col), tactile_error);
  }

  {  // Valid dimensions
    EXPECT_NO_THROW(core::tile_layer(1_row, 1_col));

    constexpr auto rows = 18_row;
    constexpr auto cols = 52_col;
    const core::tile_layer layer{rows, cols};
    EXPECT_EQ(rows, layer.row_count());
    EXPECT_EQ(cols, layer.col_count());
  }
}

TEST(TileLayer, ForEach)
{
  core::tile_layer layer;
  auto count = 0;

  layer.for_each([&](const tile_id) {
    ++count;
  });

  EXPECT_EQ(count, layer.tile_count());
}

TEST(TileLayer, RemoveAll)
{
  core::tile_layer layer;
  std::vector<core::position> positions;

  layer.flood({}, 1_t, positions);
  layer.for_each([](const tile_id id) {
    EXPECT_EQ(1_t, id);
  });

  layer.remove_all(1_t);
  layer.for_each([](const tile_id id) {
    EXPECT_EQ(empty, id);
  });
}

TEST(TileLayer, AddRow)
{
  const auto nRows = 5_row;
  const auto id = 84_t;

  core::tile_layer layer{nRows, 3_col};
  layer.add_row(id);

  EXPECT_EQ(nRows + 1_row, layer.row_count());

  for (auto col = 0_col; col < layer.col_count(); ++col) {
    EXPECT_EQ(empty, layer.tile_at({nRows - 1_row, col}));
    EXPECT_EQ(id, layer.tile_at({nRows, col}));
  }
}

TEST(TileLayer, AddCol)
{
  const auto nCols = 7_col;
  const auto id = 33_t;

  core::tile_layer layer{5_row, nCols};
  layer.add_col(id);

  EXPECT_EQ(nCols + 1_col, layer.col_count());

  for (auto row = 0_row; row < layer.row_count(); ++row) {
    EXPECT_EQ(empty, layer.tile_at({row, nCols - 1_col}));
    EXPECT_EQ(id, layer.tile_at({row, nCols}));
  }
}

TEST(TileLayer, RemoveRow)
{
  core::tile_layer layer{5_row, 5_col};

  for (int i = 0; i < 10; ++i) {
    EXPECT_NO_THROW(layer.remove_row());
  }

  EXPECT_EQ(1_row, layer.row_count());
}

TEST(TileLayer, RemoveCol)
{
  core::tile_layer layer{5_row, 5_col};

  for (int i = 0; i < 10; ++i) {
    EXPECT_NO_THROW(layer.remove_col());
  }

  EXPECT_EQ(1_col, layer.col_count());
}

TEST(TileLayer, SetRows)
{
  core::tile_layer layer;
  EXPECT_EQ(5_row, layer.row_count());
  EXPECT_EQ(5_col, layer.col_count());

  layer.set_rows(32_row);
  EXPECT_EQ(32_row, layer.row_count());
  EXPECT_EQ(5_col, layer.col_count());

  layer.set_rows(15_row);
  EXPECT_EQ(15_row, layer.row_count());
  EXPECT_EQ(5_col, layer.col_count());
}

TEST(TileLayer, SetCols)
{
  core::tile_layer layer;
  EXPECT_EQ(5_row, layer.row_count());
  EXPECT_EQ(5_col, layer.col_count());

  layer.set_cols(41_col);
  EXPECT_EQ(5_row, layer.row_count());
  EXPECT_EQ(41_col, layer.col_count());

  layer.set_cols(8_col);
  EXPECT_EQ(5_row, layer.row_count());
  EXPECT_EQ(8_col, layer.col_count());
}

TEST(TileLayer, SetTile)
{
  core::tile_layer layer;

  const core::position pos{2_row, 2_col};
  EXPECT_EQ(empty, layer.tile_at(pos));

  layer.set_tile(pos, 24_t);
  EXPECT_EQ(24_t, layer.tile_at(pos));

  EXPECT_NO_THROW(layer.set_tile({-1_row, -1_col}, 5_t));
  EXPECT_NO_THROW(layer.set_tile({layer.row_count(), layer.col_count()}, 7_t));

  EXPECT_FALSE(layer.tile_at({layer.row_count(), layer.col_count()}));
}

TEST(TileLayer, SetOpacity)
{
  core::tile_layer layer;
  EXPECT_EQ(1.0, layer.opacity());

  {  // Valid opacity
    layer.set_opacity(0.75);
    EXPECT_EQ(0.75, layer.opacity());
  }

  {  // Underflow opacity
    layer.set_opacity(-1);
    EXPECT_EQ(0, layer.opacity());
  }

  {  // Overflow opacity
    layer.set_opacity(1.1);
    EXPECT_EQ(1, layer.opacity());
  }
}

TEST(TileLayer, SetVisible)
{
  core::tile_layer layer;

  EXPECT_TRUE(layer.visible());

  layer.set_visible(false);
  EXPECT_FALSE(layer.visible());

  layer.set_visible(true);
  EXPECT_TRUE(layer.visible());
}

TEST(TileLayer, RowCount)
{
  const core::tile_layer layer{7_row, 12_col};
  EXPECT_EQ(7_row, layer.row_count());
}

TEST(TileLayer, ColCount)
{
  const core::tile_layer layer{7_row, 12_col};
  EXPECT_EQ(12_col, layer.col_count());
}

TEST(TileLayer, TileCount)
{
  const core::tile_layer layer{4_row, 5_col};
  EXPECT_EQ(20, layer.tile_count());
}

TEST(TileLayer, TileAt)
{
  const core::tile_layer layer;
  EXPECT_TRUE(layer.tile_at({0_row, 0_col}));
  EXPECT_TRUE(layer.tile_at({4_row, 4_col}));
  EXPECT_FALSE(layer.tile_at({4_row, 5_col}));
  EXPECT_FALSE(layer.tile_at({5_row, 4_col}));
  EXPECT_FALSE(layer.tile_at({5_row, 5_col}));
}

TEST(TileLayer, InBounds)
{
  const core::tile_layer layer;
  EXPECT_TRUE(layer.in_bounds({0_row, 0_col}));
  EXPECT_TRUE(layer.in_bounds({4_row, 4_col}));
  EXPECT_FALSE(layer.in_bounds({4_row, 5_col}));
  EXPECT_FALSE(layer.in_bounds({5_row, 4_col}));
  EXPECT_FALSE(layer.in_bounds({5_row, 5_col}));
}
