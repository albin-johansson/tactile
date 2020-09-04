#include "layer.hpp"

#include <catch.hpp>

#include "algorithm.hpp"
#include "position.hpp"

using namespace tactile;
using namespace tactile::model;

TEST_CASE("layer(int, int)", "[layer]")
{
  CHECK_NOTHROW(layer{-1, -1});
  const layer layer{0, 0};
  CHECK(layer.rows() == 1);
  CHECK(layer.cols() == 1);
}

TEST_CASE("layer::add_row", "[layer]")
{
  const auto nRows = 5;
  const tile_id tileID{84};

  layer layer{nRows, 3};
  layer.add_row(tileID);

  CHECK(layer.rows() == (nRows + 1));

  for (int i = 0; i < layer.cols(); ++i) {
    CHECK(*layer.tile_at({nRows - 1, i}) == empty);
    CHECK(*layer.tile_at({nRows, i}) == tileID);
  }
}

TEST_CASE("layer::add_col", "[layer]")
{
  const auto nCols = 7;
  const tile_id tileID{33};

  layer layer{5, nCols};
  layer.add_col(tileID);

  CHECK(layer.cols() == (nCols + 1));

  for (int i = 0; i < layer.rows(); ++i) {
    CHECK(*layer.tile_at({i, nCols - 1}) == empty);
    CHECK(*layer.tile_at({i, nCols}) == tileID);
  }
}

TEST_CASE("layer::remove_row", "[layer]")
{
  layer layer{5, 5};
  for (int i = 0; i < 10; ++i) {
    layer.remove_row();  // shouldn't throw
  }
  CHECK(layer.rows() == 1);
}

TEST_CASE("layer::remove_col", "[layer]")
{
  layer layer{5, 5};
  for (int i = 0; i < 10; ++i) {
    layer.remove_col();  // shouldn't throw
  }
  CHECK(layer.cols() == 1);
}

TEST_CASE("layer::set_rows", "[layer]")
{
  layer layer{5, 5};

  CHECK(layer.rows() == 5);
  CHECK(layer.cols() == 5);

  layer.set_rows(32);
  CHECK(layer.rows() == 32);
  CHECK(layer.cols() == 5);

  layer.set_rows(15);
  CHECK(layer.rows() == 15);
  CHECK(layer.cols() == 5);
}

TEST_CASE("layer::set_cols", "[layer]")
{
  layer layer{5, 5};

  CHECK(layer.rows() == 5);
  CHECK(layer.cols() == 5);

  layer.set_cols(41);
  CHECK(layer.rows() == 5);
  CHECK(layer.cols() == 41);

  layer.set_cols(8);
  CHECK(layer.rows() == 5);
  CHECK(layer.cols() == 8);
}

TEST_CASE("layer::set_tile", "[layer]")
{
  layer layer{5, 5};

  const position pos{2, 2};
  CHECK(*layer.tile_at(pos) == empty);

  const tile_id tileID{24};
  layer.set_tile(pos, tileID);

  CHECK(*layer.tile_at(pos) == tileID);

  CHECK_NOTHROW(layer.set_tile({-1, -1}, tile_id{5}));
  CHECK_NOTHROW(layer.set_tile({layer.rows(), layer.cols()}, tile_id{7}));
  CHECK(layer.tile_at({layer.rows(), layer.cols()}) == std::nullopt);
}

TEST_CASE("layer::set_visible", "[layer]")
{
  layer layer{5, 5};

  CHECK(layer.visible());

  layer.set_visible(false);
  CHECK(!layer.visible());

  layer.set_visible(true);
  CHECK(layer.visible());
}

TEST_CASE("layer::tile_at", "[layer]")
{
  const layer layer{5, 5};
  CHECK(layer.tile_at({0, 0}) != std::nullopt);
  CHECK(layer.tile_at({4, 4}) != std::nullopt);
  CHECK(layer.tile_at({5, 5}) == std::nullopt);
}

TEST_CASE("layer::in_bounds", "[layer]")
{
  const layer layer{5, 5};
  CHECK(layer.in_bounds({0, 0}));
  CHECK(layer.in_bounds({4, 4}));
  CHECK(!layer.in_bounds({5, 5}));
}