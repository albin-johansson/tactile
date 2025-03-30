// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use glam::Vec2;
use std::{cell::RefCell, rc::Rc};

/// Provides Tactile error codes.
#[repr(u8)]
#[derive(Copy, Clone, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum Errc {
  Unknown,
  BadParam,
  CouldNotCompress,
  CouldNotDecompress,
}

/// Alias for a [`Result`] using [`Errc`] as the error type.
pub type Expected<T> = Result<T, Errc>;

/// Alias for a reference-counted pointer type with dynamically checked borrow rules.
pub type Shared<T> = Rc<RefCell<T>>;

/// A two-dimensional position and size.
#[derive(Copy, Clone, Debug, Default, PartialEq)]
pub struct Bounds {
  pub pos: Vec2,
  pub size: Vec2,
}

mod attr;
pub use attr::{Attr, AttrKind, Color};

mod attr_set;
pub use attr_set::AttrSet;

mod extent;
pub use extent::Extent;

mod tile_pos;
pub use tile_pos::TilePos;
