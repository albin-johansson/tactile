// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use derive_more::{Add, AddAssign, Sub, SubAssign};

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
pub struct TextureId(u32);

