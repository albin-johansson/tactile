// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use glam::{IVec2, IVec3, IVec4, Vec2, Vec3, Vec4};
use std::path::PathBuf;

/// Represents supported attribute types.
#[repr(u8)]
#[derive(Copy, Clone, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum AttrKind {
  Int,
  Int2,
  Int3,
  Int4,
  Float,
  Float2,
  Float3,
  Float4,
  Bool,
  String,
  Path,
  Color,
}

/// Represents an 8-bit RGBA color.
#[derive(Copy, Clone, Debug, Default, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub struct Color {
  /// The red component.
  pub r: u8,

  /// The green component.
  pub g: u8,

  /// The blue component.
  pub b: u8,

  /// The alpha component.
  pub a: u8,
}

impl Color {
  /// Creates a color.
  pub const fn new(r: u8, g: u8, b: u8, a: u8) -> Self {
    Self { r, g, b, a }
  }

  pub const WHITE: Color = Self::new(0xFF, 0xFF, 0xFF, 0xFF);
  pub const BLACK: Color = Self::new(0x00, 0x00, 0x00, 0xFF);
}

/// Trait used to initialize the underlying value of [`Attr`] variants.
trait DefaultAttrValue {
  fn default_attr_value() -> Self;
}

impl DefaultAttrValue for i32 {
  fn default_attr_value() -> Self {
    0
  }
}

impl DefaultAttrValue for IVec2 {
  fn default_attr_value() -> Self {
    IVec2::splat(0)
  }
}

impl DefaultAttrValue for IVec3 {
  fn default_attr_value() -> Self {
    IVec3::splat(0)
  }
}

impl DefaultAttrValue for IVec4 {
  fn default_attr_value() -> Self {
    IVec4::splat(0)
  }
}

impl DefaultAttrValue for f32 {
  fn default_attr_value() -> Self {
    0f32
  }
}

impl DefaultAttrValue for Vec2 {
  fn default_attr_value() -> Self {
    Vec2::splat(0f32)
  }
}

impl DefaultAttrValue for Vec3 {
  fn default_attr_value() -> Self {
    Vec3::splat(0f32)
  }
}

impl DefaultAttrValue for Vec4 {
  fn default_attr_value() -> Self {
    Vec4::splat(0f32)
  }
}

impl DefaultAttrValue for bool {
  fn default_attr_value() -> Self {
    false
  }
}

impl DefaultAttrValue for String {
  fn default_attr_value() -> Self {
    String::new()
  }
}

impl DefaultAttrValue for PathBuf {
  fn default_attr_value() -> Self {
    PathBuf::new()
  }
}

impl DefaultAttrValue for Color {
  fn default_attr_value() -> Self {
    Color::BLACK
  }
}

/// Represents an attribute value.
#[derive(Clone, Debug, PartialEq)]
pub enum Attr {
  Int(i32),
  Int2(IVec2),
  Int3(IVec3),
  Int4(IVec4),
  Float(f32),
  Float2(Vec2),
  Float3(Vec3),
  Float4(Vec4),
  Bool(bool),
  String(String),
  Path(PathBuf),
  Color(Color),
}

impl Attr {
  /// Creates an attribute.
  pub fn new(kind: AttrKind) -> Self {
    match kind {
      AttrKind::Int => Self::Int(i32::default_attr_value()),
      AttrKind::Int2 => Self::Int2(IVec2::default_attr_value()),
      AttrKind::Int3 => Self::Int3(IVec3::default_attr_value()),
      AttrKind::Int4 => Self::Int4(IVec4::default_attr_value()),
      AttrKind::Float => Self::Float(f32::default_attr_value()),
      AttrKind::Float2 => Self::Float2(Vec2::default_attr_value()),
      AttrKind::Float3 => Self::Float3(Vec3::default_attr_value()),
      AttrKind::Float4 => Self::Float4(Vec4::default_attr_value()),
      AttrKind::Bool => Self::Bool(bool::default_attr_value()),
      AttrKind::String => Self::String(String::default_attr_value()),
      AttrKind::Path => Self::Path(PathBuf::default_attr_value()),
      AttrKind::Color => Self::Color(Color::default_attr_value()),
    }
  }

  /// Resets the attribute value.
  pub fn reset(&mut self) {
    *self = Self::new(self.kind());
  }

  /// Returns a reference to the stored [`i32`] value.
  pub fn get_int(&self) -> Option<&i32> {
    match self {
      Attr::Int(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`i32`] value.
  pub fn get_int_mut(&mut self) -> Option<&mut i32> {
    match self {
      Attr::Int(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`IVec2`] value.
  pub fn get_int2(&self) -> Option<&IVec2> {
    match self {
      Attr::Int2(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`IVec2`] value.
  pub fn get_int2_mut(&mut self) -> Option<&mut IVec2> {
    match self {
      Attr::Int2(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`IVec3`] value.
  pub fn get_int3(&self) -> Option<&IVec3> {
    match self {
      Attr::Int3(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`IVec3`] value.
  pub fn get_int3_mut(&mut self) -> Option<&mut IVec3> {
    match self {
      Attr::Int3(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`IVec4`] value.
  pub fn get_int4(&self) -> Option<&IVec4> {
    match self {
      Attr::Int4(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`IVec4`] value.
  pub fn get_int4_mut(&mut self) -> Option<&mut IVec4> {
    match self {
      Attr::Int4(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`f32`] value.
  pub fn get_float(&self) -> Option<&f32> {
    match self {
      Attr::Float(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`f32`] value.
  pub fn get_float_mut(&mut self) -> Option<&mut f32> {
    match self {
      Attr::Float(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`Vec2`] value.
  pub fn get_float2(&self) -> Option<&Vec2> {
    match self {
      Attr::Float2(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`Vec2`] value.
  pub fn get_float2_mut(&mut self) -> Option<&mut Vec2> {
    match self {
      Attr::Float2(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`Vec3`] value.
  pub fn get_float3(&self) -> Option<&Vec3> {
    match self {
      Attr::Float3(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`Vec3`] value.
  pub fn get_float3_mut(&mut self) -> Option<&mut Vec3> {
    match self {
      Attr::Float3(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`Vec4`] value.
  pub fn get_float4(&self) -> Option<&Vec4> {
    match self {
      Attr::Float4(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`Vec4`] value.
  pub fn get_float4_mut(&mut self) -> Option<&mut Vec4> {
    match self {
      Attr::Float4(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`bool`] value.
  pub fn get_bool(&self) -> Option<&bool> {
    match self {
      Attr::Bool(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`bool`] value.
  pub fn get_bool_mut(&mut self) -> Option<&mut bool> {
    match self {
      Attr::Bool(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`String`] value.
  pub fn get_string(&self) -> Option<&String> {
    match self {
      Attr::String(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`String`] value.
  pub fn get_string_mut(&mut self) -> Option<&mut String> {
    match self {
      Attr::String(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`PathBuf`] value.
  pub fn get_path(&self) -> Option<&PathBuf> {
    match self {
      Attr::Path(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`PathBuf`] value.
  pub fn get_path_mut(&mut self) -> Option<&mut PathBuf> {
    match self {
      Attr::Path(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a reference to the stored [`Color`] value.
  pub fn get_color(&self) -> Option<&Color> {
    match self {
      Attr::Color(value) => Some(value),
      _ => None,
    }
  }

  /// Returns a mutable reference to the stored [`Color`] value.
  pub fn get_color_mut(&mut self) -> Option<&mut Color> {
    match self {
      Attr::Color(value) => Some(value),
      _ => None,
    }
  }

  /// Indicates whether the attribute stores a vector value.
  pub fn is_vector(&self) -> bool {
    matches!(
      self,
      Attr::Int2(_)
        | Attr::Int3(_)
        | Attr::Int4(_)
        | Attr::Float2(_)
        | Attr::Float3(_)
        | Attr::Float4(_)
    )
  }

  /// Indicates whether the attribute has the value initially assigned by [`Self::new`].
  pub fn has_default_value(&self) -> bool {
    match self {
      Attr::Int(value) => *value == i32::default_attr_value(),
      Attr::Int2(value) => *value == IVec2::default_attr_value(),
      Attr::Int3(value) => *value == IVec3::default_attr_value(),
      Attr::Int4(value) => *value == IVec4::default_attr_value(),
      Attr::Float(value) => *value == f32::default_attr_value(),
      Attr::Float2(value) => *value == Vec2::default_attr_value(),
      Attr::Float3(value) => *value == Vec3::default_attr_value(),
      Attr::Float4(value) => *value == Vec4::default_attr_value(),
      Attr::Bool(value) => *value == bool::default_attr_value(),
      Attr::String(value) => *value == String::default_attr_value(),
      Attr::Path(value) => *value == PathBuf::default_attr_value(),
      Attr::Color(value) => *value == Color::default_attr_value(),
    }
  }

  /// Returns the kind of the underlying attribute value.
  pub fn kind(&self) -> AttrKind {
    match self {
      Attr::Int(_) => AttrKind::Int,
      Attr::Int2(_) => AttrKind::Int2,
      Attr::Int3(_) => AttrKind::Int3,
      Attr::Int4(_) => AttrKind::Int4,
      Attr::Float(_) => AttrKind::Float,
      Attr::Float2(_) => AttrKind::Float2,
      Attr::Float3(_) => AttrKind::Float3,
      Attr::Float4(_) => AttrKind::Float4,
      Attr::Bool(_) => AttrKind::Bool,
      Attr::String(_) => AttrKind::String,
      Attr::Path(_) => AttrKind::Path,
      Attr::Color(_) => AttrKind::Color,
    }
  }
}

impl From<i32> for Attr {
  fn from(value: i32) -> Self {
    Self::Int(value)
  }
}

impl From<IVec2> for Attr {
  fn from(value: IVec2) -> Self {
    Self::Int2(value)
  }
}

impl From<IVec3> for Attr {
  fn from(value: IVec3) -> Self {
    Self::Int3(value)
  }
}

impl From<IVec4> for Attr {
  fn from(value: IVec4) -> Self {
    Self::Int4(value)
  }
}

impl From<f32> for Attr {
  fn from(value: f32) -> Self {
    Self::Float(value)
  }
}

impl From<Vec2> for Attr {
  fn from(value: Vec2) -> Self {
    Self::Float2(value)
  }
}

impl From<Vec3> for Attr {
  fn from(value: Vec3) -> Self {
    Self::Float3(value)
  }
}

impl From<Vec4> for Attr {
  fn from(value: Vec4) -> Self {
    Self::Float4(value)
  }
}

impl From<bool> for Attr {
  fn from(value: bool) -> Self {
    Self::Bool(value)
  }
}

impl From<String> for Attr {
  fn from(value: String) -> Self {
    Self::String(value)
  }
}

impl From<PathBuf> for Attr {
  fn from(value: PathBuf) -> Self {
    Self::Path(value)
  }
}

impl From<Color> for Attr {
  fn from(value: Color) -> Self {
    Self::Color(value)
  }
}
