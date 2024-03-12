// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/util/chrono.hpp"

namespace tactile {

/// Linear animation for single tile, represented as a sequence of tile indices.
class TileAnimation final {
 public:
  struct Frame final {
    TileIndex tile {};
    Milliseconds duration {};
  };

  /// Updates the animation
  void update();

  /// Reserves enough memory for a set amount of frames.
  void reserve_frames(usize n);

  /// Adds a new frame to the animation sequence.
  void add_frame(TileIndex tile, Milliseconds duration);

  /// Inserts a new frame at a specific index in the animation sequence.
  ///
  /// \param index the index of the frame in the animation.
  /// \param tile_index the index of the tile shown during the frame.
  /// \param duration the duration of the added frame.
  /// \return nothing if a frame was added; an error code otherwise.
  auto insert_frame(usize index, TileIndex tile_index, Milliseconds duration)
      -> Result<void>;

  /// Removes the frame at the specified index.
  ///
  /// \details
  /// The animation is reset to the first frame if the current frame is
  ///   1) the removed frame,
  ///   2) or one after the removed frame.
  /// \return nothing if a frame was removed; an error code otherwise.
  auto remove_frame(usize frame_index) -> Result<void>;

  /// Returns the frame at a specific index.
  [[nodiscard]] auto operator[](usize index) -> Frame&;
  [[nodiscard]] auto operator[](usize index) const -> const Frame&;

  /// Returns the current frame.
  [[nodiscard]] auto current_frame() const -> const Frame&;

  /// Returns the frame capacity.
  [[nodiscard]] auto capacity() const -> usize;

  /// Returns the amount of frames.
  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto begin() const noexcept { return mFrames.begin(); }
  [[nodiscard]] auto end() const noexcept { return mFrames.end(); }

 private:
  Vector<Frame> mFrames;
  usize mIndex {};
  SystemClockInstant mLastUpdate {};
};

}  // namespace tactile
