// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use crate::common::Extent;
use crate::tile::Animation;
use crate::tile::AnimationFrame;
use crate::tile::TileId;
use glam::IVec2;
use std::ops::Index;
use std::ops::IndexMut;
use std::path::PathBuf;
use std::time::Duration;
use std::time::Instant;

/// Represents a tile within a [`Tileset`].
#[derive(Clone, Debug, Default)]
pub struct Tile {
  id: TileId,
  name: String,
  animation: Option<Animation>,
}

impl Tile {
  /// Creates an empty tile.
  pub fn new(id: TileId) -> Self {
    Self {
      id,
      ..Default::default()
    }
  }

  /// Updates the state of the associated [`Animation`] (if there is one).
  pub fn update(&mut self, update_time: Instant) {
    if let Some(animation) = &mut self.animation {
      animation.update(update_time);
    }
  }

  /// Adds an [`Animation`] to this tile.
  ///
  /// The animation added by this function will feature a single frame using the associated
  /// [`TileId`].
  ///
  /// This function has no effect if the tile is already animated.
  pub fn add_animation(&mut self) {
    if self.animation.is_some() {
      return;
    }

    let mut animation = Animation::new();

    let frame = AnimationFrame::new(self.id, Duration::from_secs(1));
    animation.append_frame(frame);

    self.animation = Some(animation);
  }

  /// Removes the associated [`Animation`].
  pub fn remove_animation(&mut self) -> Option<Animation> {
    self.animation.take()
  }

  /// Returns a reference to the associated [`Animation`].
  pub fn get_animation(&self) -> Option<&Animation> {
    self.animation.as_ref()
  }

  /// Returns a mutable reference to the associated [`Animation`].
  pub fn get_animation_mut(&mut self) -> Option<&mut Animation> {
    self.animation.as_mut()
  }

  /// Updates the name of the tile.
  pub fn set_name(&mut self, name: String) {
    self.name = name;
  }

  /// Returns the name of the tile.
  pub fn get_name(&self) -> &String {
    &self.name
  }

  /// Indicates whether the tile is decorated.
  ///
  /// A tile is considered decorated if any of the following conditions are true:
  ///
  /// - The tile is animated.
  /// - The tile has any annotations.
  /// - The tile has any properties.
  /// - The tile has any components.
  ///
  /// Undecorated tiles can usually be omitted from tileset definitions in save files.
  pub fn is_decorated(&self) -> bool {
    self.animation.is_some()
  }
}

