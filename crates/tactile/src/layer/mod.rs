// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use derive_more::{Add, AddAssign, Sub, SubAssign};

/// Strong type for layer identifiers.
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
pub struct LayerId(u32);

impl LayerId {
  /// Creates a layer identifier.
  pub fn new(id: u32) -> Self {
    Self(id)
  }

  /// Returns the underlying value.
  pub fn value(self) -> u32 {
    self.0
  }
}

mod annotation;
pub use annotation::{Annotation, AnnotationKind};

