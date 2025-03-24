// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use crate::core::{Attr, AttrKind};
use std::collections::HashMap;

/// Represents a set of named [`Attr`] values.
#[derive(Clone, Debug, Default, PartialEq)]
pub struct AttrSet {
  attrs: HashMap<String, Attr>,
}

impl AttrSet {
  /// Creates an empty attribute set.
  pub fn new() -> Self {
    Self {
      attrs: HashMap::new(),
    }
  }

  /// Visits each attribute in the set.
  pub fn visit(&self, mut visitor: impl FnMut(&String, &Attr)) {
    for (name, attr) in &self.attrs {
      visitor(name, attr);
    }
  }

  /// Visits each attribute in the set.
  pub fn visit_mut(&mut self, mut visitor: impl FnMut(&String, &mut Attr)) {
    for (name, attr) in &mut self.attrs {
      visitor(name, attr);
    }
  }

  /// Inserts or replaces an attribute and returns the previous value.
  pub fn add_or_replace(&mut self, name: String, attr: Attr) -> Option<Attr> {
    self.attrs.insert(name, attr)
  }

  /// Removes an attribute from the set and returns it.
  pub fn remove(&mut self, name: &str) -> Option<Attr> {
    self.attrs.remove(name)
  }

  /// Renames an attribute in the set.
  ///
  /// This function has no effect if the new name is already used by another attribute in the set.
  pub fn rename(&mut self, old_name: &str, new_name: String) -> Option<()> {
    if self.attrs.contains_key(&new_name) {
      return None;
    }

    let attr = self.remove(old_name)?;
    let _ = self.add_or_replace(new_name, attr);

    Some(())
  }

  /// Changes the type of an attribute in the set.
  pub fn change_kind(&mut self, name: &str, kind: AttrKind) -> Option<AttrKind> {
    let attr = self.get_mut(name)?;
    let prev_kind = attr.kind();

    *attr = Attr::new(kind);
    Some(prev_kind)
  }

  /// Returns a reference to an attribute in the set.
  pub fn get(&self, name: &str) -> Option<&Attr> {
    self.attrs.get(name)
  }

  /// Returns a mutable reference to an attribute in the set.
  pub fn get_mut(&mut self, name: &str) -> Option<&mut Attr> {
    self.attrs.get_mut(name)
  }

  /// Returns the number of attributes in the set.
  pub fn len(&self) -> usize {
    self.attrs.len()
  }

  /// Indicates whether the set is empty.
  pub fn is_empty(&self) -> bool {
    self.attrs.is_empty()
  }
}
