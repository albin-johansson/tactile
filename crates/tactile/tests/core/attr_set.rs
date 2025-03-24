// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use googletest::prelude::{
  assert_that, contains, eq, is_false, is_true, none, some,
};
use tactile::core::{Attr, AttrKind, AttrSet};

#[test]
fn new() {
  let set = AttrSet::new();

  assert_that!(set.len(), eq(0));
  assert_that!(set.is_empty(), is_true());
}

#[test]
fn visit() {
  let mut set = AttrSet::new();

  let name1 = "1".to_owned();
  let name2 = "2".to_owned();

  let value1 = Attr::from(1);
  let value2 = Attr::from(2);

  assert_that!(set.add_or_replace(name1.clone(), value1.clone()), none());
  assert_that!(set.add_or_replace(name2.clone(), value2.clone()), none());

  let mut names = Vec::new();
  let mut values = Vec::new();
  set.visit(|name, value| {
    names.push(name.clone());
    values.push(value.clone());
  });

  assert_that!(names, contains(eq(&name1)));
  assert_that!(names, contains(eq(&name2)));

  assert_that!(values, contains(eq(&value1)));
  assert_that!(values, contains(eq(&value2)));
}

#[test]
fn visit_mut() {
  let mut set = AttrSet::new();

  let name1 = "1".to_owned();
  let name2 = "2".to_owned();

  let value1 = Attr::from(1);
  let value2 = Attr::from(2);

  assert_that!(set.add_or_replace(name1.clone(), value1.clone()), none());
  assert_that!(set.add_or_replace(name2.clone(), value2.clone()), none());

  let mut names = Vec::new();
  let mut values = Vec::new();
  set.visit_mut(|name, value| {
    names.push(name.clone());
    values.push(value.clone());
  });

  assert_that!(names, contains(eq(&name1)));
  assert_that!(names, contains(eq(&name2)));

  assert_that!(values, contains(eq(&value1)));
  assert_that!(values, contains(eq(&value2)));
}

#[test]
fn add_or_replace() {
  let mut set = AttrSet::new();

  let name = String::from("foobar");
  let value1 = Attr::from(1);
  let value2 = Attr::from(2);

  assert_that!(set.add_or_replace(name.clone(), value1.clone()), none());
  assert_that!(set.get(&name), some(eq(&value1)));
  assert_that!(set.len(), eq(1));

  assert_that!(
    set.add_or_replace(name.clone(), value2.clone()),
    some(eq(&value1))
  );
  assert_that!(set.get(&name), some(eq(&value2)));
  assert_that!(set.len(), eq(1));
}

#[test]
fn remove() {
  let mut set = AttrSet::new();

  let name = String::from("foobar");
  let value = Attr::from(42);

  assert_that!(set.add_or_replace(name.clone(), value.clone()), none());
  assert_that!(set.len(), eq(1));

  assert_that!(set.remove(&name), some(eq(&value)));
  assert_that!(set.len(), eq(0));

  assert_that!(set.remove(&name), none());
  assert_that!(set.len(), eq(0));
}

#[test]
fn rename() {
  let mut set = AttrSet::new();

  let key1 = String::from("foo");
  let key2 = String::from("bar");
  let val = Attr::from("123".to_owned());

  assert_that!(set.add_or_replace(key1.clone(), val.clone()), none());
  assert_that!(set.get(&key1), some(eq(&val)));
  assert_that!(set.get(&key2), none());
  assert_that!(set.len(), eq(1));

  assert_that!(set.rename(&key1, key2.clone()), some(()));
  assert_that!(set.get(&key1), none());
  assert_that!(set.get(&key2), some(eq(&val)));
  assert_that!(set.len(), eq(1));

  assert_that!(set.rename(&key2, key1.clone()), some(()));
  assert_that!(set.get(&key1), some(eq(&val)));
  assert_that!(set.get(&key2), none());
  assert_that!(set.len(), eq(1));
}

#[test]
fn change_kind() {
  let mut set = AttrSet::new();

  let key = String::from("foo");
  let val = Attr::from(42);

  assert_that!(set.add_or_replace(key.clone(), val.clone()), none());
  assert_that!(set.get(&key).unwrap().has_default_value(), is_false());

  assert_that!(
    set.change_kind(&key, AttrKind::Bool),
    some(eq(AttrKind::Int))
  );
  assert_that!(set.get(&key).unwrap().has_default_value(), is_true());

  assert_that!(
    set.change_kind(&key, AttrKind::String),
    some(eq(AttrKind::Bool))
  );
  assert_that!(set.get(&key).unwrap().has_default_value(), is_true());
}
