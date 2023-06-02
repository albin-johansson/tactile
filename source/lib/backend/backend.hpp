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

#include <SDL2/SDL.h>

#include "common/result.hpp"

namespace tactile {

/// Interface for ImGui rendering backends.
class Backend {
 public:
  virtual ~Backend() noexcept = default;

  virtual void process_event(const SDL_Event* event) = 0;

  /// Attempts to begin a new frame.
  /// \return `success` if rendering can proceed; `failure` otherwise.
  virtual auto new_frame() -> Result = 0;

  /// Ends the current frame.
  /// \note This function can only be called after a successful call to `new_frame`.
  virtual void end_frame() = 0;

  /// Attempts to reload all font resources.
  virtual void reload_font_resources() {}

  /// Indicates whether the backend supports reloading font resources.
  [[nodiscard]] virtual auto can_reload_fonts() const -> bool = 0;
};

}  // namespace tactile
