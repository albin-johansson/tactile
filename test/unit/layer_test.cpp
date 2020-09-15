#include "layer.hpp"

#include <catch.hpp>

#include "algorithm.hpp"
#include "position.hpp"

using namespace tactile;
using namespace tactile::core;

TEST_CASE("layer(int, int)", "[layer]")
{
  CHECK_NOTHROW(layer{-1_row, -1_col});
  const layer layer{0_row, 0_col};
  CHECK(layer.rows() == 1_row);
  CHECK(layer.cols() == 1_col);
}

TEST_CASE("layer::add_row", "[layer]")
{
  const auto nRows = 5_row;
  const tile_id tileID{84};

  layer layer{nRows, 3_col};
  layer.add_row(tileID);

  CHECK(layer.rows() == (nRows + 1_row));

  for (auto c = 0_col; c < layer.cols(); ++c) {
    CHECK(*layer.tile_at({nRows - 1_row, c}) == empty);
    CHECK(*layer.tile_at({nRows, c}) == tileID);
  }
}

TEST_CASE("layer::add_col", "[layer]")
{
  const auto nCols = 7_col;
  const tile_id tileID{33};

  layer layer{5_row, nCols};
  layer.add_col(tileID);

  CHECK(layer.cols() == (nCols + 1_col));

  for (auto r = 0_row; r < layer.rows(); ++r) {
    CHECK(*layer.tile_at({r, nCols - 1_col}) == empty);
    CHECK(*layer.tile_at({r, nCols}) == tileID);
  }
}

TEST_CASE("layer::remove_row", "[layer]")
{
  layer layer{5_row, 5_col};
  for (int i = 0; i < 10; ++i) {
    layer.remove_row();  // shouldn't throw
  }
  CHECK(layer.rows() == 1_row);
}

TEST_CASE("layer::remove_col", "[layer]")
{
  layer layer{5_row, 5_col};
  for (int i = 0; i < 10; ++i) {
    layer.remove_col();  // shouldn't throw
  }
  CHECK(layer.cols() == 1_col);
}

TEST_CASE("layer::set_rows", "[layer]")
{
  layer layer{5_row, 5_col};

  CHECK(layer.rows() == 5_row);
  CHECK(layer.cols() == 5_col);

  layer.set_rows(32_row);
  CHECK(layer.rows() == 32_row);
  CHECK(layer.cols() == 5_col);

  layer.set_rows(15_row);
  CHECK(layer.rows() == 15_row);
  CHECK(layer.cols() == 5_col);
}

TEST_CASE("layer::set_cols", "[layer]")
{
  layer layer{5_row, 5_col};

  CHECK(layer.rows() == 5_row);
  CHECK(layer.cols() == 5_col);

  layer.set_cols(41_col);
  CHECK(layer.rows() == 5_row);
  CHECK(layer.cols() == 41_col);

  layer.set_cols(8_col);
  CHECK(layer.rows() == 5_row);
  CHECK(layer.cols() == 8_col);
}

TEST_CASE("layer::set_tile", "[layer]")
{
  layer layer{5_row, 5_col};

  const position pos{2_row, 2_col};
  CHECK(*layer.tile_at(pos) == empty);

  const tile_id tileID{24};
  layer.set_tile(pos, tileID);

  CHECK(*layer.tile_at(pos) == tileID);

  CHECK_NOTHROW(layer.set_tile({-1_row, -1_col}, tile_id{5}));
  CHECK_NOTHROW(
      layer.set_tile({row_t{layer.rows()}, col_t{layer.cols()}}, tile_id{7}));
  CHECK(layer.tile_at({row_t{layer.rows()}, col_t{layer.cols()}}) == std::nullopt);
}

TEST_CASE("layer::set_visible", "[layer]")
{
  layer layer{5_row, 5_col};

  CHECK(layer.visible());

  layer.set_visible(false);
  CHECK(!layer.visible());

  layer.set_visible(true);
  CHECK(layer.visible());
}

TEST_CASE("layer::tile_at", "[layer]")
{
  const layer layer{5_row, 5_col};
  CHECK(layer.tile_at({0_row, 0_col}) != std::nullopt);
  CHECK(layer.tile_at({4_row, 4_col}) != std::nullopt);
  CHECK(layer.tile_at({5_row, 5_col}) == std::nullopt);
}

TEST_CASE("layer::in_bounds", "[layer]")
{
  const layer layer{5_row, 5_col};
  CHECK(layer.in_bounds({0_row, 0_col}));
  CHECK(layer.in_bounds({4_row, 4_col}));
  CHECK(!layer.in_bounds({5_row, 5_col}));
}