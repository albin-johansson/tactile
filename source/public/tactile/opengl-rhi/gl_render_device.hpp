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

#include "tactile/core/containers/smart_ptr.hpp"
#include "tactile/core/platform/window.hpp"
#include "tactile/core/rhi/render_device.hpp"
#include "tactile/opengl-rhi/api.hpp"

struct SDL_Window;

namespace tactile {

struct TACTILE_OPENGL_API GLOpenGLContextDeleter final {
  void operator()(void* context) noexcept;
};

struct TACTILE_OPENGL_API GLImGuiContextDeleter final {
  void operator()(ImGuiContext* context) noexcept;
};

class TACTILE_OPENGL_API GLRenderDevice final : public IRenderDevice {
 public:
  using OpenGLContextPtr = UniquePtr<void, GLOpenGLContextDeleter>;
  using ImGuiContextPtr = UniquePtr<ImGuiContext, GLImGuiContextDeleter>;

  void initialize(Window* window) override;

  void process_imgui_event(const SDL_Event& event) override;

  auto begin_frame() -> bool override;

  void end_frame() override;

  [[nodiscard]] auto get_imgui_context() -> ImGuiContext* override;

 private:
  Window* mWindow {};
  OpenGLContextPtr mOpenGLContext;
  ImGuiContextPtr mImGuiContext;
};

}  // namespace tactile