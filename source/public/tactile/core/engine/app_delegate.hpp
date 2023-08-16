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

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/core.hpp"

union SDL_Event;

namespace tactile {

class TACTILE_CORE_API IAppDelegate {
 public:
  virtual ~IAppDelegate() noexcept = default;

  /** Called once just before the app starts running. */
  virtual void on_start() = 0;

  /** Called once immediately after the app stops running. */
  virtual void on_exit() = 0;

  /** Called for each received event. */
  virtual void on_event(const SDL_Event& event) = 0;

  /** Updates the state of the application. */
  virtual void update() = 0;

  /** Indicates whether the app is actively running. */
  [[nodiscard]] virtual auto is_running() const -> bool = 0;
};

}  // namespace tactile
