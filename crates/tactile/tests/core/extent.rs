// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use googletest::{
  assert_that,
  prelude::{eq, is_false, is_true},
};
use tactile::core::Extent;

#[test]
fn new() {
  let extent = Extent::new(15, 10);
  assert_that!(extent.width, eq(15));
  assert_that!(extent.height, eq(10));
}

#[test]
fn area() {
  assert_that!(Extent::new(0, 0).area(), eq(0));
  assert_that!(Extent::new(1, 0).area(), eq(0));
  assert_that!(Extent::new(0, 1).area(), eq(0));
  assert_that!(Extent::new(1, 1).area(), eq(1));
  assert_that!(Extent::new(2, 3).area(), eq(6));
  assert_that!(Extent::new(3, 3).area(), eq(9));
}

#[test]
fn has_area() {
  assert_that!(Extent::new(0, 0).has_area(), is_false());
  assert_that!(Extent::new(1, 0).has_area(), is_false());
  assert_that!(Extent::new(-1, 0).has_area(), is_false());
  assert_that!(Extent::new(0, 1).has_area(), is_false());
  assert_that!(Extent::new(0, -1).has_area(), is_false());
  assert_that!(Extent::new(1, -1).has_area(), is_false());
  assert_that!(Extent::new(-1, 1).has_area(), is_false());

  assert_that!(Extent::new(1, 1).has_area(), is_true());
  assert_that!(Extent::new(12, 34).has_area(), is_true());
}

#[test]
fn contains() {
  let extent = Extent::new(10, 8);

  assert_that!(extent.contains(-1, 0), is_false());
  assert_that!(extent.contains(0, -1), is_false());
  assert_that!(extent.contains(-1, -1), is_false());
  assert_that!(extent.contains(10, 0), is_false());
  assert_that!(extent.contains(0, 8), is_false());
  assert_that!(extent.contains(10, 8), is_false());

  assert_that!(extent.contains(0, 0), is_true());
  assert_that!(extent.contains(0, 7), is_true());
  assert_that!(extent.contains(9, 0), is_true());
  assert_that!(extent.contains(9, 7), is_true());
  assert_that!(extent.contains(4, 5), is_true());
}
