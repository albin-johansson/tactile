#include "tile_layer.hpp"

#include <catch.hpp>

#include "map_position.hpp"

using namespace tactile;

TEST_CASE("TileLayer(int, int)", "[TileLayer]")
{
  CHECK_NOTHROW(tile_layer{-1, -1});
  const tile_layer layer{0, 0};
  CHECK(layer.rows() == 1);
  CHECK(layer.cols() == 1);
}

TEST_CASE("TileLayer::add_row", "[TileLayer]")
{
  const auto nRows = 5;
  const auto tileID = 84;

  tile_layer layer{nRows, 3};
  layer.add_row(tileID);

  CHECK(layer.rows() == (nRows + 1));

  for (int i = 0; i < layer.cols(); ++i) {
    CHECK(*layer.tile_at({nRows - 1, i}) == empty);
    CHECK(*layer.tile_at({nRows, i}) == tileID);
  }
}

TEST_CASE("TileLayer::add_col", "[TileLayer]")
{
  const auto nCols = 7;
  const auto tileID = 33;

  tile_layer layer{5, nCols};
  layer.add_col(tileID);

  CHECK(layer.cols() == (nCols + 1));

  for (int i = 0; i < layer.rows(); ++i) {
    CHECK(*layer.tile_at({i, nCols - 1}) == empty);
    CHECK(*layer.tile_at({i, nCols}) == tileID);
  }
}

TEST_CASE("TileLayer::remove_row", "[TileLayer]")
{
  tile_layer layer{5, 5};
  for (int i = 0; i < 10; ++i) {
    layer.remove_row();  // shouldn't throw
  }
  CHECK(layer.rows() == 1);
}

TEST_CASE("TileLayer::remove_col", "[TileLayer]")
{
  tile_layer layer{5, 5};
  for (int i = 0; i < 10; ++i) {
    layer.remove_col();  // shouldn't throw
  }
  CHECK(layer.cols() == 1);
}

TEST_CASE("TileLayer::set_tile", "[TileLayer]")
{
  tile_layer layer{5, 5};

  const map_position pos{2, 2};
  CHECK(*layer.tile_at(pos) == empty);

  const auto tileID = 24;
  layer.set_tile(pos, tileID);

  CHECK(*layer.tile_at(pos) == tileID);

  CHECK_NOTHROW(layer.set_tile({-1, -1}, 5));
  CHECK_NOTHROW(layer.set_tile({layer.rows(), layer.cols()}, 7));
  CHECK(layer.tile_at({layer.rows(), layer.cols()}) == nothing);
}

TEST_CASE("TileLayer::set_visible", "[TileLayer]")
{
  tile_layer layer{5, 5};

  CHECK(layer.visible());

  layer.set_visible(false);
  CHECK(!layer.visible());

  layer.set_visible(true);
  CHECK(layer.visible());
}

TEST_CASE("TileLayer::tile_at", "[TileLayer]")
{
  const tile_layer layer{5, 5};
  CHECK(layer.tile_at({0, 0}) != nothing);
  CHECK(layer.tile_at({4, 4}) != nothing);
  CHECK(layer.tile_at({5, 5}) == nothing);
}

TEST_CASE("TileLayer::in_bounds", "[TileLayer]")
{
  const tile_layer layer{5, 5};
  CHECK(layer.in_bounds({0, 0}));
  CHECK(layer.in_bounds({4, 4}));
  CHECK(!layer.in_bounds({5, 5}));
}