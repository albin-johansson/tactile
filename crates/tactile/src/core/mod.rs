// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use derive_more::{Add, AddAssign, Sub, SubAssign};

/// Represents a two-dimensional tile position.
///
/// Note that tile positions with negative coordinates are allowed.
#[derive(
  Copy,
  Clone,
  Debug,
  Default,
  Eq,
  PartialEq,
  Ord,
  PartialOrd,
  Hash,
  Add,
  AddAssign,
  Sub,
  SubAssign,
)]
pub struct TilePos {
  /// The index along the x-axis.
  pub x: isize,

  /// The index along the y-axis.
  pub y: isize,
}

impl TilePos {
  /// Creates a tile position.
  pub const fn new(x: isize, y: isize) -> Self {
    Self { x, y }
  }

  /// Creates a tile position from a one-dimensional index and the underlying grid width.
  ///
  /// # Panics
  ///
  /// Panics if `width` is zero.
  pub const fn from_index(index: isize, width: isize) -> Self {
    Self::new(index % width, index / width)
  }

  /// Returns a tile position that references the adjacent left neighbor.
  pub const fn west(&self) -> Self {
    Self::new(self.x - 1, self.y)
  }

  /// Returns a tile position that references the adjacent right neighbor.
  pub const fn east(&self) -> Self {
    Self::new(self.x + 1, self.y)
  }

  /// Returns a tile position that references the adjacent above neighbor.
  pub const fn north(&self) -> Self {
    Self::new(self.x, self.y - 1)
  }

  /// Returns a tile position that references the adjacent below neighbor.
  pub const fn south(&self) -> Self {
    Self::new(self.x, self.y + 1)
  }

  /// Indicates whether the tile position has positive coordinates.
  pub const fn is_positive(&self) -> bool {
    self.x >= 0 && self.y >= 0
  }
}

/// Represents a two-dimensional tile grid extent.
///
/// Note that tile grids with negative dimensions are allowed.
#[derive(
  Copy,
  Clone,
  Debug,
  Default,
  Eq,
  PartialEq,
  Ord,
  PartialOrd,
  Hash,
  Add,
  AddAssign,
  Sub,
  SubAssign,
)]
pub struct TileGrid {
  /// The column count.
  pub width: isize,

  /// The row count.
  pub height: isize,
}

impl TileGrid {
  /// Creates a tile grid extent.
  pub const fn new(width: isize, height: isize) -> Self {
    Self { width, height }
  }

  /// Indicates whether the grid has a positive area.
  pub const fn has_area(&self) -> bool {
    self.width > 0 && self.height > 0
  }

  /// Indicates whether the grid contains a tile position.
  pub const fn contains(&self, pos: TilePos) -> bool {
    self.has_area() && pos.is_positive() && pos.x < self.width && pos.y < self.height
  }
}

mod attr;
pub use attr::{Attr, AttrKind, Color};
