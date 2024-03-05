/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/type/result.hpp"
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
  /// \return success if a frame was added; failure otherwise.
  auto insert_frame(usize index, TileIndex tile_index, Milliseconds duration) -> Result;

  /// Removes the frame at the specified index.
  ///
  /// \details
  /// The animation is reset to the first frame if the current frame is
  ///   1) the removed frame,
  ///   2) or one after the removed frame.
  /// \return success if a frame was removed; failure otherwise.
  auto remove_frame(usize frame_index) -> Result;

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
