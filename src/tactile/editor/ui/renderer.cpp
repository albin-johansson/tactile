// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/ui/renderer.hpp"

#include <cassert>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdlgpu3.h>

#include "tactile/core/log/logger.hpp"

namespace tactile::editor {

Renderer::Renderer(SDL_Window& window, SDL_GPUDevice& device)
  : m_window {&window},
    m_device {&device}
{}

auto Renderer::begin_frame() -> bool
{
  if ((SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED) != 0u) {
    return false;
  }

  m_current_command_buffer = SDL_AcquireGPUCommandBuffer(m_device);

  if (m_current_command_buffer == nullptr) {
    log_error("Could not acquire GPU command buffer: {}", SDL_GetError());
    return false;
  }

  if (!SDL_WaitAndAcquireGPUSwapchainTexture(m_current_command_buffer,
                                             m_window,
                                             &m_current_swapchain_texture,
                                             nullptr,
                                             nullptr)) {
    log_error("Could not acquire GPU swapchain texture: {}", SDL_GetError());
    return false;
  }

  return true;
}

void Renderer::render()
{
  assert(m_current_command_buffer != nullptr);
  assert(m_current_swapchain_texture != nullptr);

  ImGui::Render();
  auto* const draw_data = ImGui::GetDrawData();

  ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, m_current_command_buffer);

  SDL_GPUColorTargetInfo color_target_info {};
  color_target_info.texture = m_current_swapchain_texture;
  color_target_info.clear_color = SDL_FColor {0, 0, 0, 1};
  color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
  color_target_info.store_op = SDL_GPU_STOREOP_STORE;
  color_target_info.mip_level = 0;
  color_target_info.layer_or_depth_plane = 0;
  color_target_info.cycle = false;

  if (auto* const render_pass = SDL_BeginGPURenderPass(m_current_command_buffer,
                                                       &color_target_info,
                                                       1,
                                                       nullptr)) {
    ImGui_ImplSDLGPU3_RenderDrawData(draw_data, m_current_command_buffer, render_pass);
    SDL_EndGPURenderPass(render_pass);
  }
  else {
    // The SDL_BeginGPURenderPass documentation doesn't mention any error conditions, so
    // don't include SDL_GetError output to avoid misleading error messages.
    log_error("Could not begin GPU render pass");
  }

  if (!SDL_SubmitGPUCommandBuffer(m_current_command_buffer)) {
    log_error("Could not submit GPU command buffer: {}", SDL_GetError());
  }

  m_current_swapchain_texture = nullptr;
  m_current_command_buffer = nullptr;
}

}  // namespace tactile::editor
