// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

/// Strong type for global tile identifiers.
#[derive(Copy, Clone, Debug, Ord, PartialOrd, Eq, PartialEq, Hash)]
pub struct TileId(i32);

/// The identifier used by empty tiles.
pub const EMPTY_TILE_ID: TileId = TileId(0);

