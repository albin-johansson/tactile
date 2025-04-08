// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use googletest::prelude::assert_that;
use googletest::prelude::eq;
use tactile::common::Attr;
use tactile::common::Component;
use tactile::common::ComponentDef;
use tactile::common::ComponentId;

#[test]
fn from_component_def() {
  let id = ComponentId::new(42);

  let comp_name = String::from("xyz");
  let attr1_name = String::from("1");
  let attr2_name = String::from("2");
  let attr3_name = String::from("3");

  let attr1 = Attr::from(100);
  let attr2 = Attr::from(true);
  let attr3 = Attr::from("foo".to_owned());

  let mut comp_def = ComponentDef::new(id);
  comp_def.set_name(comp_name.clone());

  {
    let comp_def_attrs = comp_def.attrs_mut();
    let _ = comp_def_attrs.add_or_replace(attr1_name.clone(), attr1.clone());
    let _ = comp_def_attrs.add_or_replace(attr2_name.clone(), attr2.clone());
    let _ = comp_def_attrs.add_or_replace(attr3_name.clone(), attr3.clone());
  }

  let comp = Component::from(&comp_def);
  let comp_def_attrs = comp_def.attrs();
  let comp_attrs = comp.attrs();

  assert_that!(comp.id(), eq(id));
  assert_that!(comp_attrs.len(), eq(comp_def_attrs.len()));

  assert_that!(
    comp_attrs.get(attr1_name.as_str()),
    eq(comp_def_attrs.get(attr1_name.as_str()))
  );

  assert_that!(
    comp_attrs.get(attr2_name.as_str()),
    eq(comp_def_attrs.get(attr2_name.as_str()))
  );

  assert_that!(
    comp_attrs.get(attr3_name.as_str()),
    eq(comp_def_attrs.get(attr3_name.as_str()))
  );
}
