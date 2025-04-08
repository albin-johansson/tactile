// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

use glam::Vec2;
use std::cell::RefCell;
use std::rc::Rc;

/// Alias for a [`Result`] using [`Error`] as the error type.
pub type Expected<T> = Result<T, Error>;

/// Alias for a reference-counted pointer type with dynamically checked borrow rules.
pub type Shared<T> = Rc<RefCell<T>>;

/// A two-dimensional position and size.
#[derive(Copy, Clone, Debug, Default, PartialEq)]
pub struct Bounds {
  pub pos: Vec2,
  pub size: Vec2,
}

mod attr;
mod attr_set;
mod component;
mod error;
mod extent;
mod meta;
mod opacity;
mod tile_pos;

pub use attr::{Attr, AttrKind, Color};
pub use attr_set::AttrSet;
pub use component::{Component, ComponentDef, ComponentId, ComponentSet};
pub use error::Error;
pub use extent::Extent;
pub use meta::{MetaCtx, Metadata};
pub use opacity::Opacity;
pub use tile_pos::TilePos;
