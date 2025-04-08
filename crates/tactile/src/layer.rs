// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use crate::common::Result;
use crate::common::Extent;
use crate::common::Metadata;
use crate::common::Opacity;
use derive_more::Add;
use derive_more::AddAssign;
use derive_more::Sub;
use derive_more::SubAssign;

mod annotation;

pub use annotation::{Annotation, AnnotationKind};

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

