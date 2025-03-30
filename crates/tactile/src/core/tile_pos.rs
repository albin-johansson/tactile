// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use derive_more::{Add, AddAssign, Sub, SubAssign};

use crate::core::Extent;

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

  /// Converts the tile position to the corresponding 1-dimensional index within an extent.
  ///
  /// # Examples
  ///
  /// ```
  /// use tactile::core::{Extent, TilePos};
  ///
  /// let extent = Extent::new(3, 3);
  ///
  /// assert_eq!(TilePos::new(0, 0).to_index(extent).unwrap(), 0);
  /// assert_eq!(TilePos::new(1, 0).to_index(extent).unwrap(), 1);
  /// assert_eq!(TilePos::new(2, 0).to_index(extent).unwrap(), 2);
  /// assert_eq!(TilePos::new(0, 1).to_index(extent).unwrap(), 3);
  /// ```
  pub const fn to_index(&self, extent: Extent) -> Option<isize> {
    if !extent.contains(self.x, self.y) {
      None
    } else {
      Some(self.y * extent.width + self.x)
    }
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
