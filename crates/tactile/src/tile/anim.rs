// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use crate::common::Error;
use crate::common::Expected;
use crate::tile::TileId;
use std::time::Duration;
use std::time::Instant;

/// Represents a single frame in an [`Animation`].
#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub struct AnimationFrame {
  tile_id: TileId,
  duration: Duration,
}

impl AnimationFrame {
  /// Creates an animation frame.
  pub const fn new(tile_id: TileId, duration: Duration) -> Self {
    Self { tile_id, duration }
  }

  /// Returns the identifier of the tile shown when the frame is active.
  pub const fn get_tile_id(&self) -> TileId {
    self.tile_id
  }

  /// Returns the amount of time that the frame is shown.
  pub const fn get_duration(&self) -> Duration {
    self.duration
  }
}

/// Represents a tile animation.
#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Animation {
  frames: Vec<AnimationFrame>,
  current_frame: usize,
  last_update_time: Instant,
}

impl Animation {
  /// Creates an empty animation.
  pub fn new() -> Self {
    Self {
      frames: Vec::new(),
      current_frame: 0,
      last_update_time: Instant::now(),
    }
  }

  /// Updates the state of the animation.
  ///
  /// The time of the update, `update_time`, is passed as an explicit parameter to try to minimize
  /// the number of calls to [`Instant::now`] when calling `update` on several [`Animation`]
  /// instances in rapid succession.
  pub fn update(&mut self, update_time: Instant) {
    if let Some(frame) = self.frames.get(self.current_frame) {
      if update_time - self.last_update_time >= frame.duration {
        self.current_frame = (self.current_frame + 1) % self.frames.len();
        self.last_update_time = update_time;
      }
    }
  }

  /// Inserts a frame into the animation.
  ///
  /// It's possible to insert frames at the end of the animation by setting `index` to the length of
  /// the animation. The animation will be reset if the specified frame is successfully inserted.
  pub fn insert_frame(
    &mut self,
    index: usize,
    frame: AnimationFrame,
  ) -> Expected<()> {
    let frame_count = self.len();

    if index > frame_count {
      return Err(Error::BadParam);
    }

    if index == frame_count {
      self.frames.push(frame);
    } else {
      self.frames.insert(index, frame);
    }

    self.reset();
    Ok(())
  }

  /// Adds a frame to the end of the animation.
  pub fn append_frame(&mut self, frame: AnimationFrame) {
    self.insert_frame(self.len(), frame).unwrap();
  }

  /// Removes a frame from the animation.
  ///
  /// The animation will be reset if the specified frame is successfully removed.
  pub fn remove_frame(&mut self, index: usize) -> Option<AnimationFrame> {
    if index >= self.len() {
      return None;
    }

    let frame = self.frames.remove(index);
    self.reset();

    Some(frame)
  }

  /// Returns the identifier of the currently active tile.
  pub fn get_current_tile_id(&self) -> Option<TileId> {
    self.frames.get(self.current_frame)?.tile_id.into()
  }

  /// Returns the number of frames in the animation.
  pub fn len(&self) -> usize {
    self.frames.len()
  }

  /// Indicates whether the animation is empty, i.e., whether it has no frames.
  pub fn is_empty(&self) -> bool {
    self.frames.is_empty()
  }

  fn reset(&mut self) {
    self.last_update_time = Instant::now();
    self.current_frame = 0;
  }
}

impl Default for Animation {
  fn default() -> Self {
    Self::new()
  }
}
