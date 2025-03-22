// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use googletest::{
  assert_that,
  prelude::{eq, is_false, is_true, lt, none, some},
};
use tactile::tile::{EMPTY_TILE_ID, MAX_USER_TILE_ID, MIN_USER_TILE_ID, TileId};

#[test]
fn empty() {
  assert_that!(TileId::empty(), eq(EMPTY_TILE_ID));
}

#[test]
fn new() {
  assert_that!(TileId::new(MIN_USER_TILE_ID.value() - 1), none());
  assert_that!(TileId::new(MAX_USER_TILE_ID.value() + 1), none());

  assert_that!(
    TileId::new(MIN_USER_TILE_ID.value()),
    some(eq(MIN_USER_TILE_ID))
  );

  assert_that!(
    TileId::new(MAX_USER_TILE_ID.value()),
    some(eq(MAX_USER_TILE_ID))
  );

  assert_that!(TileId::new(42).unwrap().value(), eq(42));
}

#[test]
fn is_valid() {
  assert_that!(TileId::is_valid(MIN_USER_TILE_ID.value() - 1), is_false());
  assert_that!(TileId::is_valid(MAX_USER_TILE_ID.value() + 1), is_false());
  assert_that!(TileId::is_valid(EMPTY_TILE_ID.value()), is_false());

  assert_that!(TileId::is_valid(MIN_USER_TILE_ID.value()), is_true());
  assert_that!(TileId::is_valid(MAX_USER_TILE_ID.value()), is_true());
  assert_that!(TileId::is_valid(42), is_true());
}

#[test]
fn empty_tile_id() {
  assert_that!(EMPTY_TILE_ID.value(), eq(0));
  assert_that!(EMPTY_TILE_ID, lt(MIN_USER_TILE_ID));
}

#[test]
fn min_user_tile_id() {
  assert_that!(MIN_USER_TILE_ID.value(), eq(1));
}

#[test]
fn max_user_tile_id() {
  assert_that!(MAX_USER_TILE_ID.value(), eq(0x00_FF_FF_FF));
}
