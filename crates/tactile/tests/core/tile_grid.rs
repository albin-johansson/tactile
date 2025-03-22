// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use googletest::{
  assert_that,
  prelude::{eq, is_false, is_true},
};
use tactile::core::{TileGrid, TilePos};

#[test]
fn new() {
  let grid = TileGrid::new(15, 10);
  assert_that!(grid.width, eq(15));
  assert_that!(grid.height, eq(10));
}

#[test]
fn has_area() {
  assert_that!(TileGrid::new(0, 0).has_area(), is_false());
  assert_that!(TileGrid::new(1, 0).has_area(), is_false());
  assert_that!(TileGrid::new(-1, 0).has_area(), is_false());
  assert_that!(TileGrid::new(0, 1).has_area(), is_false());
  assert_that!(TileGrid::new(0, -1).has_area(), is_false());
  assert_that!(TileGrid::new(1, -1).has_area(), is_false());
  assert_that!(TileGrid::new(-1, 1).has_area(), is_false());

  assert_that!(TileGrid::new(1, 1).has_area(), is_true());
  assert_that!(TileGrid::new(12, 34).has_area(), is_true());
}

#[test]
fn contains() {
  let grid = TileGrid::new(10, 8);

  assert_that!(grid.contains(TilePos::new(-1, 0)), is_false());
  assert_that!(grid.contains(TilePos::new(0, -1)), is_false());
  assert_that!(grid.contains(TilePos::new(-1, -1)), is_false());
  assert_that!(grid.contains(TilePos::new(10, 0)), is_false());
  assert_that!(grid.contains(TilePos::new(0, 8)), is_false());
  assert_that!(grid.contains(TilePos::new(10, 8)), is_false());

  assert_that!(grid.contains(TilePos::new(0, 0)), is_true());
  assert_that!(grid.contains(TilePos::new(0, 7)), is_true());
  assert_that!(grid.contains(TilePos::new(9, 0)), is_true());
  assert_that!(grid.contains(TilePos::new(9, 7)), is_true());
  assert_that!(grid.contains(TilePos::new(4, 5)), is_true());
}
