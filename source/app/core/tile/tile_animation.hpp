/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <chrono>  // system_clock, milliseconds

#include "core/type/vector.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Linear animation for single tile, represented as a sequence of tile indices.
class TileAnimation final {
 public:
  using Millis = std::chrono::milliseconds;
  using Clock = std::chrono::system_clock;
  using TimePoint = Clock::time_point;

  struct Frame final {
    TileIndex tile {};
    Millis duration {};
  };

  /// Updates the animation
  void update();

  /// Reserves enough memory for a set amount of frames.
  void reserve_frames(usize n);

  /// Adds a new frame to the animation sequence.
  void add_frame(TileIndex tile, Millis duration);

  /// Returns the frame at a specific index.
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
  Vec<Frame> mFrames;
  usize mIndex {};
  TimePoint mLastUpdate {};
};

}  // namespace tactile
