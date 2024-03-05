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

#include <centurion/opengl.hpp>
#include <centurion/window.hpp>

#include "tactile/base/prelude.hpp"

namespace tactile {

/// Handles the configuration of the Dear ImGui context.
class ImGuiContext final {
 public:
  TACTILE_DEFAULT_COPY(ImGuiContext);
  TACTILE_DEFAULT_MOVE(ImGuiContext);

  ImGuiContext(cen::window& window, cen::gl_context& context);

  ~ImGuiContext();

  /// Reloads the main and icon fonts.
  ///
  /// This is meant to be called every time the application has reason to believe
  /// that the fonts need to be reloaded, e.g. when the reported display framebuffer size
  /// changes.
  static void reload_fonts();

 private:
  /// This is used as a workaround to annoying unit test issues for Windows CI builds
  bool mInitializedBackend {};
};

}  // namespace tactile
