// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

/// Strong type for global tile identifiers.
#[derive(Copy, Clone, Debug, Ord, PartialOrd, Eq, PartialEq, Hash)]
pub struct TileId(i32);

/// The smallest valid identifier for user tiles.
pub const MIN_USER_TILE_ID: TileId = TileId(1);

/// The largest valid identifier for user tiles.
///
/// # Details
///
/// Tiled uses the upper 4 bits of tile identifiers to encode ["flip" flags][1]. Tactile doesn't use
/// this encoding scheme, but the bits are unused for compatibility reasons. This means that the
/// largest possible valid tile identifier is `16_777_215 - 1`.
///
/// Tiled assigns the following meaning to the upper 4 bits.
/// - Bit 31: Whether the tile is horizontally flipped.
/// - Bit 30: Whether the tile is vertically flipped.
/// - Bit 29: For isometric and orthogonal maps, whether the tile is flipped diagonally. In
///           hexagonal maps, whether the tile is rotated 60 degrees clockwise.
/// - Bit 28: In hexagonal maps, whether the tile is rotated 120 degrees clockwise.
///
/// [1]: https://doc.mapeditor.org/en/stable/reference/global-tile-ids/#gid-tile-flipping
pub const MAX_USER_TILE_ID: TileId = TileId(0x00_FF_FF_FF);

impl TileId {
  /// Returns the empty tile identifier.
  pub const fn empty() -> Self {
    Self(0)
  }

  /// Creates a tile identifier.
  pub const fn new(id: i32) -> Option<Self> {
    if Self::is_valid(id) {
      Some(Self(id))
    } else {
      None
    }
  }

  /// Indicates whether a value is a valid non-empty tile identifier.
  ///
  /// See [`MIN_USER_TILE_ID`] and [`MAX_USER_TILE_ID`].
  pub const fn is_valid(id: i32) -> bool {
    id >= MIN_USER_TILE_ID.value() && id <= MAX_USER_TILE_ID.value()
  }

  /// Returns the underlying value of the tile identifier.
  pub const fn value(&self) -> i32 {
    self.0
  }
}

/// The identifier used by empty tiles.
///
/// Note, this is smaller than [`MIN_USER_TILE_ID`].
pub const EMPTY_TILE_ID: TileId = TileId::empty();

impl Default for TileId {
  fn default() -> Self {
    EMPTY_TILE_ID
  }
}

