// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <SDL3/SDL.h>

namespace tactile::editor {

class Renderer final
{
 public:
  Renderer(SDL_Window& window, SDL_GPUDevice& device);

  /// Tries to acquire a command buffer and swapchain texture (returns true on success).
  auto begin_frame() -> bool;

  /// Renders the primitives from Dear ImGui and submits the command buffer.
  ///
  /// Note, every call to this function must have been preceded by a successful call to
  /// `begin_frame`.
  void render();

 private:
  SDL_Window* m_window;
  SDL_GPUDevice* m_device;
  SDL_GPUCommandBuffer* m_current_command_buffer {nullptr};
  SDL_GPUTexture* m_current_swapchain_texture {nullptr};
};

}  // namespace tactile::editor
