// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use crate::core::{AttrSet, ComponentSet};

/// A collection of metadata.
#[derive(Clone, Debug, Default)]
pub struct Metadata {
  attrs: AttrSet,
  comps: ComponentSet,
}

impl Metadata {
  /// Returns a reference to the associated set of properties.
  pub fn props(&self) -> &AttrSet {
    &self.attrs
  }

  /// Returns a mutable reference to the associated set of properties.
  pub fn props_mut(&mut self) -> &mut AttrSet {
    &mut self.attrs
  }

  /// Returns a reference to the associated set of components.
  pub fn comps(&self) -> &ComponentSet {
    &self.comps
  }

  /// Returns a mutable reference to the associated set of components.
  pub fn comps_mut(&mut self) -> &mut ComponentSet {
    &mut self.comps
  }
}

/// A trait for types that feature associated [`Metadata`].
pub trait MetaCtx {
  /// Returns a reference to the associated [`Metadata`].
  fn metadata(&self) -> &Metadata;

  /// Returns a mutable reference to the associated [`Metadata`].
  fn metadata_mut(&mut self) -> &mut Metadata;
}
