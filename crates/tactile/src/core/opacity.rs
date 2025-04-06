// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use std::ops::Add;
use std::ops::AddAssign;
use std::ops::Sub;
use std::ops::SubAssign;

/// Represents an 8-bit opacity value.
#[derive(Copy, Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub struct Opacity(u8);

impl Opacity {
  /// Minimum opacity (completely transparent).
  pub const MIN: Opacity = Opacity(0);

  /// Maximum opacity (completely opaque).
  pub const MAX: Opacity = Opacity(255);

  /// Returns the underlying value.
  pub const fn value(self) -> u8 {
    self.0
  }

  /// Returns the underlying value as a normalized [`f32`].
  pub const fn normalized(self) -> f32 {
    self.0 as f32 / 255f32
  }
}

impl From<u8> for Opacity {
  fn from(value: u8) -> Self {
    Self(value)
  }
}

impl From<f32> for Opacity {
  fn from(value: f32) -> Self {
    Self::from((value.clamp(0f32, 1f32) * 255f32) as u8)
  }
}

impl Default for Opacity {
  fn default() -> Self {
    Self::MAX
  }
}

impl Add for Opacity {
  type Output = Self;

  fn add(self, rhs: Self) -> Self::Output {
    Self(self.0.saturating_add(rhs.0))
  }
}

impl AddAssign for Opacity {
  fn add_assign(&mut self, rhs: Self) {
    *self = self.add(rhs);
  }
}

impl Sub for Opacity {
  type Output = Self;

  fn sub(self, rhs: Self) -> Self::Output {
    Self(self.0.saturating_sub(rhs.0))
  }
}

impl SubAssign for Opacity {
  fn sub_assign(&mut self, rhs: Self) {
    *self = self.sub(rhs);
  }
}
