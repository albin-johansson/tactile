// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use derive_more::Add;
use derive_more::AddAssign;
use derive_more::Sub;
use derive_more::SubAssign;
use glam::IVec2;

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
  pub const fn new(id: u32) -> Self {
    Self(id)
  }

  /// Returns the underlying value.
  pub const fn value(self) -> u32 {
    self.0
  }
}

/// Represents a reference to a texture.
#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub struct Texture {
  id: TextureId,
  size: IVec2,
}

impl Texture {
  /// Creates a new `Texture`.
  pub const fn new(id: TextureId, size: IVec2) -> Self {
    Self { id, size }
  }

  /// Returns the associated [`TextureId`].
  pub const fn id(&self) -> TextureId {
    self.id
  }

  /// Returns the size of the texture.
  pub const fn size(&self) -> IVec2 {
    self.size
  }
}
