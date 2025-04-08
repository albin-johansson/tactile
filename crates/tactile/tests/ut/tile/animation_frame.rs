// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use googletest::{assert_that, prelude::eq};
use std::time::Duration;
use tactile::tile::{AnimationFrame, TileId};

#[test]
fn new() {
  const TILE_ID: TileId = TileId::new(42).unwrap();
  const DURATION: Duration = Duration::from_millis(123);

  let frame = AnimationFrame::new(TILE_ID, DURATION);
  assert_that!(frame.get_tile_id(), eq(TILE_ID));
  assert_that!(frame.get_duration(), eq(DURATION));
}
