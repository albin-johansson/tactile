// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use googletest::{
  assert_that,
  prelude::{eq, is_false, is_true, lt, none, some},
};
use tactile::tile::{EMPTY_TILE_ID, MAX_USER_TILE_ID, MIN_USER_TILE_ID, TileId};

#[test]
fn is_valid_non_empty_tile() {
  assert_that!(
    TileId::is_valid_non_empty_tile(MIN_USER_TILE_ID.value() - 1),
    is_false()
  );

  assert_that!(
    TileId::is_valid_non_empty_tile(MAX_USER_TILE_ID.value() + 1),
    is_false()
  );

  assert_that!(
    TileId::is_valid_non_empty_tile(EMPTY_TILE_ID.value()),
    is_false()
  );

  assert_that!(
    TileId::is_valid_non_empty_tile(MIN_USER_TILE_ID.value()),
    is_true()
  );

  assert_that!(
    TileId::is_valid_non_empty_tile(MAX_USER_TILE_ID.value()),
    is_true()
  );

  assert_that!(TileId::is_valid_non_empty_tile(42), is_true());
}

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
fn from_tiled() {
  const TILED_FLIP_H_BIT: i32 = 1 << 31;
  const TILED_FLIP_V_BIT: i32 = 1 << 30;
  const TILED_FLIP_D_BIT: i32 = 1 << 29;
  const TILED_ROTATE_120_BIT: i32 = 1 << 28;
  const TILED_ALL_FLIP_BITS: i32 =
    TILED_FLIP_H_BIT | TILED_FLIP_V_BIT | TILED_FLIP_D_BIT | TILED_ROTATE_120_BIT;

  let tile_id = TileId::new(42).unwrap();
  let tile_id_flip_h = tile_id.value() | TILED_FLIP_H_BIT;
  let tile_id_flip_v = tile_id.value() | TILED_FLIP_V_BIT;
  let tile_id_flip_d = tile_id.value() | TILED_FLIP_V_BIT;
  let tile_id_rotate_120 = tile_id.value() | TILED_ROTATE_120_BIT;
  let tile_id_flip_all = tile_id.value() | TILED_ALL_FLIP_BITS;

  assert_that!(TileId::from_tiled(tile_id.value()), some(eq(tile_id)));
  assert_that!(TileId::from_tiled(tile_id_flip_h), some(eq(tile_id)));
  assert_that!(TileId::from_tiled(tile_id_flip_v), some(eq(tile_id)));
  assert_that!(TileId::from_tiled(tile_id_flip_d), some(eq(tile_id)));
  assert_that!(TileId::from_tiled(tile_id_rotate_120), some(eq(tile_id)));
  assert_that!(TileId::from_tiled(tile_id_flip_all), some(eq(tile_id)));

  assert_that!(
    TileId::from_tiled(EMPTY_TILE_ID.value()),
    some(eq(EMPTY_TILE_ID))
  );

  assert_that!(
    TileId::from_tiled(MIN_USER_TILE_ID.value()),
    some(eq(MIN_USER_TILE_ID))
  );

  assert_that!(
    TileId::from_tiled(MAX_USER_TILE_ID.value()),
    some(eq(MAX_USER_TILE_ID))
  );
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
