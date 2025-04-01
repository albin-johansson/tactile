// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use crate::core::{Bounds, MetaCtx, Metadata};

/// Represents different kinds of annotation objects.
#[repr(u8)]
#[derive(Copy, Clone, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum AnnotationKind {
  Point,
  Rect,
  Ellipse,
}

/// Represents an annotation object.
#[derive(Clone, Debug)]
pub struct Annotation {
  pub kind: AnnotationKind,
  pub name: String,
  pub bounds: Bounds,
  pub metadata: Metadata,
  pub visible: bool,
}

impl Annotation {
  pub fn new(kind: AnnotationKind) -> Self {
    Self {
      kind,
      name: String::default(),
      bounds: Bounds::default(),
      metadata: Metadata::default(),
      visible: true,
    }
  }
}

impl MetaCtx for Annotation {
  fn metadata(&self) -> &Metadata {
    &self.metadata
  }

  fn metadata_mut(&mut self) -> &mut Metadata {
    &mut self.metadata
  }
}
