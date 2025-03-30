// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use derive_more::{Add, AddAssign, Sub, SubAssign};

/// Strong type for texture identifiers.
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
pub struct TextureId(u32);

impl TextureId {
  /// Creates a texture identifier.
  pub fn new(id: u32) -> Self {
    Self(id)
  }

  /// Returns the underlying value.
  pub fn value(self) -> u32 {
    self.0
  }
}

