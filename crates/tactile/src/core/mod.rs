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

/// Represents a two-dimensional extent.
///
/// Note that extents with negative dimensions are allowed.
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
pub struct Extent {
  /// The extent width.
  pub width: isize,

  /// The extent height.
  pub height: isize,
}

impl Extent {
  /// Creates an extent.
  pub const fn new(width: isize, height: isize) -> Self {
    Self { width, height }
  }

  /// Converts a [`TilePos`] to the corresponding 1-dimensional index within the extent.
  ///
  /// # Examples
  ///
  /// ```
  /// use tactile::core::{Extent, TilePos};
  ///
  /// let extent = Extent::new(3, 3);
  ///
  /// assert_eq!(extent.pos_to_idx(TilePos::new(0, 0)).unwrap(), 0);
  /// assert_eq!(extent.pos_to_idx(TilePos::new(1, 0)).unwrap(), 1);
  /// assert_eq!(extent.pos_to_idx(TilePos::new(2, 0)).unwrap(), 2);
  /// assert_eq!(extent.pos_to_idx(TilePos::new(0, 1)).unwrap(), 3);
  /// ```
  pub const fn pos_to_idx(&self, pos: TilePos) -> Option<isize> {
    if !self.contains(pos.x, pos.y) {
      None
    } else {
      Some(pos.y * self.width + pos.x)
    }
  }

  /// Returns the area of the extent.
  pub const fn area(&self) -> isize {
    self.width * self.height
  }

  /// Indicates whether the extent has a non-negative area.
  pub const fn has_area(&self) -> bool {
    self.width > 0 && self.height > 0
  }

  /// Indicates whether the extent contains a given point.
  pub const fn contains(&self, x: isize, y: isize) -> bool {
    self.has_area() && x >= 0 && y >= 0 && x < self.width && y < self.height
  }
}

mod attr;
pub use attr::{Attr, AttrKind, Color};
