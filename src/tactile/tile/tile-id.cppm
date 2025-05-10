// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.tile:id;

export import tactile.core;

export namespace tactile {

/// Alias for global tile identifiers.
using TileID = i32;

/// The tile identifier used by empty tiles.
inline constexpr TileID kEmptyTileId {0};

}  // namespace tactile
