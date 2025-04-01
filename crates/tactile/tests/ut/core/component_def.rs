// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use googletest::{
  assert_that,
  prelude::{eq, is_true},
};
use tactile::core::{ComponentDef, ComponentId};

#[test]
fn new() {
  let id = ComponentId::new(42);
  let mut def = ComponentDef::new(id);

  assert_that!(def.id(), eq(id));
  assert_that!(def.name().is_empty(), is_true());
  assert_that!(def.attrs().is_empty(), is_true());
  assert_that!(def.attrs_mut().is_empty(), is_true());
}

#[test]
fn set_name() {
  let id = ComponentId::new(42);
  let mut def = ComponentDef::new(id);

  let name = String::from("foobar");
  def.set_name(name.clone());

  assert_that!(def.name(), eq(&name));
}
