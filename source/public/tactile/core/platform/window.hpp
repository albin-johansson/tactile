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
#include "tactile/core/containers/smart_ptr.hpp"
#include "tactile/core/core.hpp"
#include "tactile/core/rhi/graphics_api.hpp"

struct SDL_Window;

namespace tactile {

struct TACTILE_CORE_API WindowDeleter final {
  void operator()(SDL_Window* window) noexcept;
};

class TACTILE_CORE_API Window final {
 public:
  using UniqueWindow = UniquePtr<SDL_Window, WindowDeleter>;

  /** Creates a window for the specified graphics API. */
  explicit Window(GraphicsAPI api);

  void show();

  void hide();

  void maximize();

  [[nodiscard]] auto get_width() const -> int;

  [[nodiscard]] auto get_height() const -> int;

  [[nodiscard]] auto get_graphics_api() const -> GraphicsAPI;

  [[nodiscard]] auto get_handle() -> SDL_Window*;

 private:
  GraphicsAPI mAPI {};
  UniqueWindow mWindow;
};

}  // namespace tactile
