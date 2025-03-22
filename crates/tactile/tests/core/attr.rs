// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use glam::{IVec2, IVec3, IVec4, Vec2, Vec3, Vec4};
use googletest::{
  assert_that,
  prelude::{eq, is_false, is_true, some},
};
use std::path::PathBuf;
use tactile::core::{Attr, AttrKind, Color};

#[test]
fn from_int() {
  let value = 123;
  let attr = Attr::from(value);
  assert_that!(attr.get_int(), some(eq(&value)));
}

#[test]
fn from_int2() {
  let value = IVec2::new(12, 34);
  let attr = Attr::from(value);
  assert_that!(attr.get_int2(), some(eq(&value)));
}

#[test]
fn from_int3() {
  let value = IVec3::new(12, 34, 56);
  let attr = Attr::from(value);
  assert_that!(attr.get_int3(), some(eq(&value)));
}

#[test]
fn from_int4() {
  let value = IVec4::new(12, 34, 56, 78);
  let attr = Attr::from(value);
  assert_that!(attr.get_int4(), some(eq(&value)));
}

#[test]
fn from_float() {
  let value = 123f32;
  let attr = Attr::from(value);
  assert_that!(attr.get_float(), some(eq(&value)));
}

#[test]
fn from_float2() {
  let value = Vec2::new(12f32, 34f32);
  let attr = Attr::from(value);
  assert_that!(attr.get_float2(), some(eq(&value)));
}

#[test]
fn from_float3() {
  let value = Vec3::new(12f32, 34f32, 56f32);
  let attr = Attr::from(value);
  assert_that!(attr.get_float3(), some(eq(&value)));
}

#[test]
fn from_float4() {
  let value = Vec4::new(12f32, 34f32, 56f32, 78f32);
  let attr = Attr::from(value);
  assert_that!(attr.get_float4(), some(eq(&value)));
}

#[test]
fn from_bool() {
  let value = true;
  let attr = Attr::from(value);
  assert_that!(attr.get_bool(), some(eq(&value)));
}

#[test]
fn from_string() {
  let value = String::from("foobar");
  let attr = Attr::from(value.clone());
  assert_that!(attr.get_string(), some(eq(&value)));
}

#[test]
fn from_path() {
  let value = PathBuf::from("foo/bar");
  let attr = Attr::from(value.clone());
  assert_that!(attr.get_path(), some(eq(&value)));
}

#[test]
fn from_color() {
  let value = Color::new(0x11, 0x22, 0x33, 0x44);
  let attr = Attr::from(value);
  assert_that!(attr.get_color(), some(eq(&value)));
}

#[test]
fn new_int() {
  let attr = Attr::new(AttrKind::Int);
  assert_that!(attr.kind(), eq(AttrKind::Int));

  assert_that!(attr.is_vector(), is_false());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr.get_int(), some(eq(&0)));
}

#[test]
fn new_int2() {
  let attr = Attr::new(AttrKind::Int2);
  assert_that!(attr.kind(), eq(AttrKind::Int2));

  assert_that!(attr.is_vector(), is_true());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(IVec2::default())));
}

#[test]
fn new_int3() {
  let attr = Attr::new(AttrKind::Int3);
  assert_that!(attr.kind(), eq(AttrKind::Int3));

  assert_that!(attr.is_vector(), is_true());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(IVec3::default())));
}

#[test]
fn new_int4() {
  let attr = Attr::new(AttrKind::Int4);
  assert_that!(attr.kind(), eq(AttrKind::Int4));

  assert_that!(attr.is_vector(), is_true());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(IVec4::default())));
}

#[test]
fn new_float() {
  let attr = Attr::new(AttrKind::Float);
  assert_that!(attr.kind(), eq(AttrKind::Float));

  assert_that!(attr.is_vector(), is_false());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(f32::default())));
}

#[test]
fn new_float2() {
  let attr = Attr::new(AttrKind::Float2);
  assert_that!(attr.kind(), eq(AttrKind::Float2));

  assert_that!(attr.is_vector(), is_true());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(Vec2::default())));
}

#[test]
fn new_float3() {
  let attr = Attr::new(AttrKind::Float3);
  assert_that!(attr.kind(), eq(AttrKind::Float3));

  assert_that!(attr.is_vector(), is_true());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(Vec3::default())));
}

#[test]
fn new_float4() {
  let attr = Attr::new(AttrKind::Float4);
  assert_that!(attr.kind(), eq(AttrKind::Float4));

  assert_that!(attr.is_vector(), is_true());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(Vec4::default())));
}

#[test]
fn new_bool() {
  let attr = Attr::new(AttrKind::Bool);
  assert_that!(attr.kind(), eq(AttrKind::Bool));

  assert_that!(attr.is_vector(), is_false());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(false)));
}

#[test]
fn new_string() {
  let attr = Attr::new(AttrKind::String);
  assert_that!(attr.kind(), eq(AttrKind::String));

  assert_that!(attr.is_vector(), is_false());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(String::new())));
}

#[test]
fn new_path() {
  let attr = Attr::new(AttrKind::Path);
  assert_that!(attr.kind(), eq(AttrKind::Path));

  assert_that!(attr.is_vector(), is_false());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(PathBuf::new())));
}

#[test]
fn new_color() {
  let attr = Attr::new(AttrKind::Color);
  assert_that!(attr.kind(), eq(AttrKind::Color));

  assert_that!(attr.is_vector(), is_false());
  assert_that!(attr.has_default_value(), is_true());

  assert_that!(attr, eq(&Attr::from(Color::BLACK)));
}

#[test]
fn reset() {
  let mut attr = Attr::new(AttrKind::Int);
  assert_that!(attr.kind(), eq(AttrKind::Int));

  if let Some(value) = attr.get_int_mut() {
    *value = 42;
  }

  attr.reset();
  assert_that!(attr.kind(), eq(AttrKind::Int));
  assert_that!(attr.get_int(), some(eq(&0)));
}
