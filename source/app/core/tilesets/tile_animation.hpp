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
#include <vector>  // vector

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"

namespace tactile {

class TileAnimation final
{
 public:
  using Millis = std::chrono::milliseconds;
  using Clock = std::chrono::system_clock;
  using TimePoint = Clock::time_point;

  struct Frame final
  {
    TileIndex tile{};
    Millis    duration{};
  };

  void update();

  void reserve_frames(usize n);

  void add_frame(TileIndex tile, Millis duration);

  [[nodiscard]] auto nth(usize index) const -> const Frame&;

  [[nodiscard]] auto current_tile() const -> TileIndex;

  [[nodiscard]] auto capacity() const -> usize;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto begin() const noexcept { return mFrames.begin(); }
  [[nodiscard]] auto end() const noexcept { return mFrames.end(); }

 private:
  std::vector<Frame> mFrames;
  usize              mIndex{};
  TimePoint          mLastUpdate{};
};

}  // namespace tactile
