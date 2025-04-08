// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use derive_more::{Add, AddAssign, Sub, SubAssign};
use std::collections::HashMap;

use crate::common::AttrSet;

/// Strong type for component identifiers.
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
pub struct ComponentId(u32);

impl ComponentId {
  /// Creates a component identifier.
  pub fn new(id: u32) -> Self {
    Self(id)
  }

  /// Returns the underlying value.
  pub fn value(self) -> u32 {
    self.0
  }
}

/// Describes a component definition.
///
/// Every `ComponentDef` is uniquely identified by an associated [`ComponentId`].
#[derive(Clone, Debug, PartialEq)]
pub struct ComponentDef {
  id: ComponentId,
  name: String,
  attrs: AttrSet,
}

impl ComponentDef {
  /// Creates a component definition.
  pub fn new(id: ComponentId) -> Self {
    Self {
      id,
      name: String::new(),
      attrs: AttrSet::new(),
    }
  }

  /// Returns the associated identifier.
  pub fn id(&self) -> ComponentId {
    self.id
  }

  /// Sets the name of the component.
  pub fn set_name(&mut self, name: String) {
    self.name = name;
  }

  /// Returns a reference to the component name.
  pub fn name(&self) -> &String {
    &self.name
  }

  /// Returns a reference to the associated attributes.
  pub fn attrs(&self) -> &AttrSet {
    &self.attrs
  }

  /// Returns a mutable reference to the associated attributes.
  pub fn attrs_mut(&mut self) -> &mut AttrSet {
    &mut self.attrs
  }
}

/// A cohesive collection of named attributes.
///
/// Each [`Component`] has an associated [`ComponentDef`] from which it's instantiated. Changes to a
/// component definition should be reflected in all instantiated components of that type. However,
/// this is not automatically enforced.
#[derive(Clone, Debug, PartialEq)]
pub struct Component {
  id: ComponentId,
  attrs: AttrSet,
}

impl From<&ComponentDef> for Component {
  fn from(value: &ComponentDef) -> Self {
    Self {
      id: value.id,
      attrs: value.attrs.clone(),
    }
  }
}

impl Component {
  /// Returns the identifier of the associated [`ComponentDef`].
  pub fn id(&self) -> ComponentId {
    self.id
  }

  /// Returns a reference to the associated attributes.
  pub fn attrs(&self) -> &AttrSet {
    &self.attrs
  }

  /// Returns a mutable reference to the associated attributes.
  pub fn attrs_mut(&mut self) -> &mut AttrSet {
    &mut self.attrs
  }
}

/// A set of components.
#[derive(Clone, Debug, Default, PartialEq)]
pub struct ComponentSet {
  comps: HashMap<ComponentId, Component>,
}

impl ComponentSet {
  /// Visits each component in the set.
  pub fn visit(&self, mut visitor: impl FnMut(ComponentId, &Component)) {
    for (id, comp) in &self.comps {
      visitor(*id, comp);
    }
  }

  /// Visits each component in the set.
  pub fn visit_mut(&mut self, mut visitor: impl FnMut(ComponentId, &mut Component)) {
    for (id, comp) in &mut self.comps {
      visitor(*id, comp);
    }
  }

  /// Adds or replaces a component.
  pub fn add_or_replace(&mut self, comp: Component) -> Option<Component> {
    self.comps.insert(comp.id, comp)
  }

  /// Removes a component from the set.
  pub fn remove(&mut self, id: ComponentId) -> Option<Component> {
    self.comps.remove(&id)
  }

  /// Returns a reference to a component in the set.
  pub fn get(&self, id: ComponentId) -> Option<&Component> {
    self.comps.get(&id)
  }

  /// Returns a mutable reference to a component in the set.
  pub fn get_mut(&mut self, id: ComponentId) -> Option<&mut Component> {
    self.comps.get_mut(&id)
  }

  /// Returns the number of components in the set.
  pub fn len(&self) -> usize {
    self.comps.len()
  }

  /// Indicates whether the set is empty.
  pub fn is_empty(&self) -> bool {
    self.comps.is_empty()
  }
}
