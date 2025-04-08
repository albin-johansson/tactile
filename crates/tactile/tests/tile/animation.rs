// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use googletest::{
  assert_that,
  prelude::{eq, is_true, none, ok, some},
};
use std::time::{Duration, Instant};
use tactile::tile::{Animation, AnimationFrame, TileId};

const ZERO_DURATION: Duration = Duration::from_millis(0);

const TILE_ID1: TileId = TileId::new(1).unwrap();
const TILE_ID2: TileId = TileId::new(2).unwrap();
const TILE_ID3: TileId = TileId::new(3).unwrap();
const TILE_ID4: TileId = TileId::new(4).unwrap();

const FRAME1: AnimationFrame = AnimationFrame::new(TILE_ID1, ZERO_DURATION);
const FRAME2: AnimationFrame = AnimationFrame::new(TILE_ID2, ZERO_DURATION);
const FRAME3: AnimationFrame = AnimationFrame::new(TILE_ID3, ZERO_DURATION);
const FRAME4: AnimationFrame = AnimationFrame::new(TILE_ID4, ZERO_DURATION);

#[test]
fn new() {
  let animation = Animation::new();
  assert_that!(animation.len(), eq(0));
  assert_that!(animation.is_empty(), is_true());
  assert_that!(animation.get_current_tile_id(), none());
}

#[test]
fn update() {
  let mut animation = Animation::new();
  animation.append_frame(FRAME1);
  animation.append_frame(FRAME2);
  animation.append_frame(FRAME3);
  assert_that!(animation.len(), eq(3));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));

  // [ ->FRAME1, FRAME2, FRAME3 ] => [ FRAME1, ->FRAME2, FRAME3 ]
  animation.update(Instant::now());
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID2)));

  // [ FRAME1, ->FRAME2, FRAME3 ] => [ FRAME1, FRAME2, ->FRAME3 ]
  animation.update(Instant::now());
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID3)));

  // [ FRAME1, FRAME2, ->FRAME3 ] => [ ->FRAME1, FRAME2, FRAME3 ]
  animation.update(Instant::now());
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));
}

#[test]
fn insert_frame() {
  let mut animation = Animation::new();
  assert_that!(animation.get_current_tile_id(), none());

  // [ ] => [ ->FRAME1 ]
  assert_that!(animation.insert_frame(0, FRAME1), ok(()));
  assert_that!(animation.len(), eq(1));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));

  // [ ->FRAME1 ] => [ ->FRAME1, FRAME2 ]
  assert_that!(animation.insert_frame(1, FRAME2), ok(()));
  assert_that!(animation.len(), eq(2));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));

  // [ ->FRAME1, FRAME2 ] => [ FRAME1, ->FRAME2 ]
  animation.update(Instant::now());
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID2)));

  // [ FRAME1, ->FRAME2 ] => [ ->FRAME1, FRAME2, FRAME3 ]
  assert_that!(animation.insert_frame(2, FRAME3), ok(()));
  assert_that!(animation.len(), eq(3));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));

  // [ ->FRAME1, FRAME2, FRAME3 ] => [ FRAME1, FRAME2, ->FRAME3 ]
  animation.update(Instant::now());
  animation.update(Instant::now());
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID3)));

  //  [ FRAME1, FRAME2, ->FRAME3 ] => [ ->FRAME1, FRAME4, FRAME2, FRAME3 ]
  assert_that!(animation.insert_frame(1, FRAME4), ok(()));
  assert_that!(animation.len(), eq(4));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));
}

#[test]
fn append_frame() {
  let mut animation = Animation::new();
  animation.append_frame(FRAME1);

  assert_that!(animation.len(), eq(1));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));
}

#[test]
fn remove_frame() {
  let mut animation = Animation::new();
  animation.append_frame(FRAME1);
  animation.append_frame(FRAME2);
  animation.append_frame(FRAME3);
  assert_that!(animation.len(), eq(3));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));

  assert_that!(animation.remove_frame(2), some(eq(FRAME3)));
  assert_that!(animation.len(), eq(2));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));

  assert_that!(animation.remove_frame(1), some(eq(FRAME2)));
  assert_that!(animation.len(), eq(1));
  assert_that!(animation.get_current_tile_id(), some(eq(TILE_ID1)));

  assert_that!(animation.remove_frame(0), some(eq(FRAME1)));
  assert_that!(animation.len(), eq(0));
  assert_that!(animation.get_current_tile_id(), none());
}
