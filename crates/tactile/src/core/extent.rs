// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use derive_more::{Add, AddAssign, Sub, SubAssign};

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

  /// Returns the area of the extent.
  pub const fn area(&self) -> isize {
    self.width * self.height
  }

  /// Indicates whether the extent has a non-empty positive area.
  pub const fn has_area(&self) -> bool {
    self.width > 0 && self.height > 0
  }

  /// Indicates whether the extent contains a given point.
  pub const fn contains(&self, x: isize, y: isize) -> bool {
    self.has_area() && x >= 0 && y >= 0 && x < self.width && y < self.height
  }
}
