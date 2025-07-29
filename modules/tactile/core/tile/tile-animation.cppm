// Copyright (C) 2025 Albin Johansson

export module tactile.core.tile:animation;

import :constants;
import tactile.core.prelude;
import tactile.core.error;

export namespace tactile {

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
/// Animations are based on an initial frame that represents the parent tile.
/// It's not possible to remove this initial frame from an animation.
class TileAnimation final
{
 public:
  explicit TileAnimation(const TileAnimationFrame& first_frame);

  /// Updates the state of the animation.
  void update(SteadyClock::time_point update_time);

  /// Inserts a frame at a given position in the animation.
  ///
  /// This function can also be used to append frames if the specified index is
  /// equal to the number of frames. However, note that it's not allowed to
  /// insert frames at index 0.
  ///
  /// If successful, this function always resets the progress of the animation.
  [[nodiscard]]
  auto insert_frame(isize index, const TileAnimationFrame& frame)
      -> Result<void>;

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

}  // namespace tactile
