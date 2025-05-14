// Copyright (C) 2025 Albin Johansson

export module tactile.tile:id;

export import tactile.common;

export namespace tactile {

/// Alias for global tile identifiers.
using TileID = i32;

/// The tile identifier used by empty tiles.
inline constexpr TileID kEmptyTileId {0};

}  // namespace tactile
