// Copyright (C) 2025 Albin Johansson

/// Provides APIs related to tiles and tilesets.
export module tactile.core.tile;

export import tactile.core.common;
export import tactile.core.io;
export import tactile.core.numeric;

export namespace tactile {

/// Alias for global tile identifiers.
using TileID = i32;

/// The tile identifier used by empty tiles.
inline constexpr TileID kEmptyTileId {0};

/// Represents a single frame in a tile animation.
struct TileAnimationFrame final
{
  /// The identifier of the tile showed during the frame.
  TileID tile_id {};

  /// The length of time the frame is shown.
  Milliseconds duration {};
};

/// Represents a sequential tile animation.
///
/// Animations are based on an initial frame that represents the parent tile. It's
/// not possible to remove this initial frame from an animation.
class TileAnimation final
{
 public:
  explicit TileAnimation(const TileAnimationFrame& first_frame);

  /// Updates the state of the animation.
  void update(SteadyClock::time_point update_time);

  /// Inserts a frame at a given position in the animation.
  ///
  /// This function can also be used to append frames if the specified index is equal
  /// to the number of frames. However, note that it's not allowed to insert frames
  /// at index 0.
  ///
  /// If successful, this function always resets the progress of the animation.
  [[nodiscard]]
  auto insert_frame(isize index, const TileAnimationFrame& frame) -> Result<void>;

  /// Inserts a frame at the end of the animation.
  void append_frame(const TileAnimationFrame& frame);

  /// Removes a frame at a given index.
  ///
  /// It's not possible to remove the frame at index 0.
  [[nodiscard]]
  auto erase_frame(isize index) -> Result<TileAnimationFrame>;

  /// Returns the currently active frame.
  [[nodiscard]]
  auto current_frame() const -> const TileAnimationFrame&;

  /// Returns the number of frames in the animation.
  [[nodiscard]]
  auto frame_count() const -> usize;

 private:
  Vector<TileAnimationFrame> m_frames {};
  usize m_current_frame {0uz};
  SteadyClock::time_point m_last_update {SteadyClock::now()};
};

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
