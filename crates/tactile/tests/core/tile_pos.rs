// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use googletest::{
  assert_that,
  prelude::{eq, is_false, is_true, none, some},
};
use tactile::core::{Extent, TilePos};

#[test]
fn new() {
  let pos = TilePos::new(1, 2);
  assert_that!(pos.x, eq(1));
  assert_that!(pos.y, eq(2));
}

#[test]
fn from_index() {
  const WIDTH: isize = 3;

  assert_that!(TilePos::from_index(0, WIDTH), eq(TilePos::new(0, 0)));
  assert_that!(TilePos::from_index(1, WIDTH), eq(TilePos::new(1, 0)));
  assert_that!(TilePos::from_index(2, WIDTH), eq(TilePos::new(2, 0)));
  assert_that!(TilePos::from_index(3, WIDTH), eq(TilePos::new(0, 1)));
  assert_that!(TilePos::from_index(4, WIDTH), eq(TilePos::new(1, 1)));
  assert_that!(TilePos::from_index(5, WIDTH), eq(TilePos::new(2, 1)));
  assert_that!(TilePos::from_index(6, WIDTH), eq(TilePos::new(0, 2)));
  assert_that!(TilePos::from_index(7, WIDTH), eq(TilePos::new(1, 2)));
  assert_that!(TilePos::from_index(8, WIDTH), eq(TilePos::new(2, 2)));

  assert_that!(TilePos::from_index(-0, WIDTH), eq(TilePos::new(-0, -0)));
  assert_that!(TilePos::from_index(-1, WIDTH), eq(TilePos::new(-1, -0)));
  assert_that!(TilePos::from_index(-2, WIDTH), eq(TilePos::new(-2, -0)));
  assert_that!(TilePos::from_index(-3, WIDTH), eq(TilePos::new(-0, -1)));
  assert_that!(TilePos::from_index(-4, WIDTH), eq(TilePos::new(-1, -1)));
  assert_that!(TilePos::from_index(-5, WIDTH), eq(TilePos::new(-2, -1)));
  assert_that!(TilePos::from_index(-6, WIDTH), eq(TilePos::new(-0, -2)));
  assert_that!(TilePos::from_index(-7, WIDTH), eq(TilePos::new(-1, -2)));
  assert_that!(TilePos::from_index(-8, WIDTH), eq(TilePos::new(-2, -2)));
}

#[test]
#[should_panic]
fn from_index_panics_if_width_is_zero() {
  let _ = TilePos::from_index(0, 0);
}

#[test]
fn to_index() {
  let extent = Extent::new(3, 3);

  assert_that!(TilePos::new(0, 0).to_index(extent), some(eq(0)));
  assert_that!(TilePos::new(1, 0).to_index(extent), some(eq(1)));
  assert_that!(TilePos::new(2, 0).to_index(extent), some(eq(2)));
  assert_that!(TilePos::new(0, 1).to_index(extent), some(eq(3)));
  assert_that!(TilePos::new(1, 1).to_index(extent), some(eq(4)));
  assert_that!(TilePos::new(2, 1).to_index(extent), some(eq(5)));
  assert_that!(TilePos::new(0, 2).to_index(extent), some(eq(6)));
  assert_that!(TilePos::new(1, 2).to_index(extent), some(eq(7)));
  assert_that!(TilePos::new(2, 2).to_index(extent), some(eq(8)));

  assert_that!(TilePos::new(-1, 0).to_index(extent), none());
  assert_that!(TilePos::new(0, -1).to_index(extent), none());
  assert_that!(TilePos::new(3, 0).to_index(extent), none());
  assert_that!(TilePos::new(0, 3).to_index(extent), none());
}

#[test]
fn west() {
  assert_that!(TilePos::new(0, 0).west(), eq(TilePos::new(-1, 0)));
  assert_that!(TilePos::new(3, 3).west(), eq(TilePos::new(2, 3)));
}

#[test]
fn east() {
  assert_that!(TilePos::new(0, 0).east(), eq(TilePos::new(1, 0)));
  assert_that!(TilePos::new(3, 3).east(), eq(TilePos::new(4, 3)));
}

#[test]
fn north() {
  assert_that!(TilePos::new(0, 0).north(), eq(TilePos::new(0, -1)));
  assert_that!(TilePos::new(3, 3).north(), eq(TilePos::new(3, 2)));
}

#[test]
fn south() {
  assert_that!(TilePos::new(0, 0).south(), eq(TilePos::new(0, 1)));
  assert_that!(TilePos::new(3, 3).south(), eq(TilePos::new(3, 4)));
}

#[test]
fn is_positive() {
  assert_that!(TilePos::new(0, 0).is_positive(), is_true());

  assert_that!(TilePos::new(1, 0).is_positive(), is_true());
  assert_that!(TilePos::new(0, 1).is_positive(), is_true());
  assert_that!(TilePos::new(1, 1).is_positive(), is_true());

  assert_that!(TilePos::new(-1, 0).is_positive(), is_false());
  assert_that!(TilePos::new(0, -1).is_positive(), is_false());
  assert_that!(TilePos::new(-1, -1).is_positive(), is_false());
}
