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

#include "engine/backend/backend.hpp"

namespace tactile {

/// Represents an OpenGL renderer backend.
class OpenGLBackend final : public Backend {
 public:
  OpenGLBackend(SDL_Window* window, SDL_GLContext context);

  ~OpenGLBackend() noexcept override;

  void process_event(const SDL_Event* event) override;

  auto new_frame() -> Result override;

  void end_frame() override;

  auto reload_font_resources() -> Result override;

 private:
  SDL_Window* mWindow;
};

}  // namespace tactile
