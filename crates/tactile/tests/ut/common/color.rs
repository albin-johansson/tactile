// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use googletest::assert_that;
use googletest::prelude::eq;
use tactile::common::Color;

#[test]
fn new() {
  let color = Color::new(0x11, 0x22, 0x33, 0x44);

  assert_that!(color.r, eq(0x11));
  assert_that!(color.g, eq(0x22));
  assert_that!(color.b, eq(0x33));
  assert_that!(color.a, eq(0x44));
}

#[test]
fn constants() {
  assert_that!(Color::BLACK, eq(Color::new(0x00, 0x00, 0x00, 0xFF)));
  assert_that!(Color::WHITE, eq(Color::new(0xFF, 0xFF, 0xFF, 0xFF)));
}
