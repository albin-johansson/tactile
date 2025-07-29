// Copyright (C) 2025 Albin Johansson

export module tactile.core.tile:tile;

import :constants;
import :animation;
import tactile.core.prelude;
import tactile.core.error;

export namespace tactile {

/// Represents a tile definition.
class Tile final
{
 public:
  /// Creates a tile definition.
  explicit Tile(TileID id);

  /// Returns the associated identifier.
  [[nodiscard]]
  auto id() const -> TileID;

  /// Returns the associated animation.
  [[nodiscard]]
  auto animation() -> Option<TileAnimation>&;

  /// Returns the associated animation.
  [[nodiscard]]
  auto animation() const -> const Option<TileAnimation>&;

 private:
  TileID m_id;
  Option<TileAnimation> m_animation {};
};

}  // namespace tactile
