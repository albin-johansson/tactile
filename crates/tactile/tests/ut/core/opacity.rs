// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use googletest::prelude::approx_eq;
use googletest::prelude::assert_that;
use googletest::prelude::eq;
use tactile::core::Opacity;

#[test]
fn constants() {
  assert_that!(Opacity::MIN.value(), eq(0));
  assert_that!(Opacity::MAX.value(), eq(255));
}

#[test]
fn default() {
  let opacity = Opacity::default();
  assert_that!(opacity, eq(Opacity::MAX));
}

#[test]
fn from_u8() {
  assert_that!(Opacity::from(123).value(), eq(123));
}

#[test]
fn from_f32() {
  assert_that!(Opacity::from(0.5f32).value(), eq(127));
  assert_that!(Opacity::from(-1.0f32), eq(Opacity::MIN));
  assert_that!(Opacity::from(1.1f32), eq(Opacity::MAX));
}

#[test]
fn normalized() {
  assert_that!(Opacity::MIN.normalized(), eq(0f32));
  assert_that!(Opacity::MAX.normalized(), eq(1f32));
  assert_that!(Opacity::from(123).normalized(), approx_eq(123f32 / 255f32));
}

#[test]
fn add() {
  assert_that!((Opacity::from(100) + Opacity::from(25)).value(), eq(125));
  assert_that!(Opacity::MAX + Opacity::from(1), eq(Opacity::MAX));
}

#[test]
fn add_assign() {
  let mut opacity = Opacity::from(10);

  opacity += Opacity::from(20);
  assert_that!(opacity, eq(Opacity::from(30)));

  opacity += Opacity::MAX;
  assert_that!(opacity, eq(Opacity::MAX));
}

#[test]
fn sub() {
  assert_that!((Opacity::from(100) - Opacity::from(25)).value(), eq(75));
  assert_that!(Opacity::MIN - Opacity::from(1), eq(Opacity::MIN));
}

#[test]
fn sub_assign() {
  let mut opacity = Opacity::from(30);

  opacity -= Opacity::from(10);
  assert_that!(opacity, eq(Opacity::from(20)));

  opacity -= Opacity::MAX;
  assert_that!(opacity, eq(Opacity::MIN));
}
