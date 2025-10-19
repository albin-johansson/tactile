// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/platform/sdl.hpp"

#include <stdexcept>
#include <utility>

#include "tactile/core/log/logger.hpp"

namespace tactile::editor {

SDLContext::SDLContext()
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    log_error("Could not initialize SDL context: {}", SDL_GetError());
    throw std::runtime_error {"Could not initialize SDL context"};
  }

  (void) m_quitter.emplace([] noexcept {
    log_verbose("Destroying SDL context");
    SDL_Quit();
  });
}

void SDLWindowDeleter::operator()(SDL_Window* const window) noexcept
{
  log_verbose("Destroying SDL window");
  SDL_DestroyWindow(window);
}

void SDLGPUDeviceDeleter::operator()(SDL_GPUDevice* const device) noexcept
{
  log_verbose("Destroying SDL GPU device");
  SDL_DestroyGPUDevice(device);
}

auto make_editor_window() -> UniqueSDLWindow
{
  auto* const window = SDL_CreateWindow(
      "Tactile",
      800,
      600,
      SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

  if (window == nullptr) {
    log_error("Could not create SDL window: {}", SDL_GetError());
    throw std::runtime_error {"Could not create SDL window"};
  }

  return UniqueSDLWindow {window};
}

auto make_editor_gpu_device(SDL_Window& window,
                            const bool debug,
                            const char* const driver_name) -> SDLGPUDeviceInfo
{
  auto* const gpu_device =
      SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_METALLIB |
                              SDL_GPU_SHADERFORMAT_DXBC,
                          debug,
                          driver_name);
  if (gpu_device == nullptr) {
    log_error("Could not create GPU device: {}", SDL_GetError());
    throw std::runtime_error {"Could not create GPU device"};
  }

  SDLGPUDeviceInfo gpu_device_info {};
  gpu_device_info.device.reset(gpu_device);
  gpu_device_info.present_mode = SDL_GPU_PRESENTMODE_VSYNC;
  gpu_device_info.swapchain_composition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;

  log_debug("Selected GPU driver '{}'", SDL_GetGPUDeviceDriver(gpu_device));

  if (!SDL_ClaimWindowForGPUDevice(gpu_device, &window)) {
    log_error("Could not claim window for GPU device: {}", SDL_GetError());
    throw std::runtime_error {"Could not claim window for GPU device"};
  }

  constexpr auto ideal_present_mode = SDL_GPU_PRESENTMODE_MAILBOX;
  if (SDL_WindowSupportsGPUPresentMode(gpu_device, &window, ideal_present_mode)) {
    gpu_device_info.present_mode = ideal_present_mode;
  }

  if (!SDL_SetGPUSwapchainParameters(gpu_device,
                                     &window,
                                     gpu_device_info.swapchain_composition,
                                     gpu_device_info.present_mode)) {
    log_error("Could not set swapchain parameters: {}", SDL_GetError());
    throw std::runtime_error {"Could not set swapchain parameters"};
  }

  log_debug("GPU device present mode: {}",
            std::to_underlying(gpu_device_info.present_mode));
  log_debug("GPU device swapchain composition: {}",
            std::to_underlying(gpu_device_info.swapchain_composition));

  return gpu_device_info;
}

}  // namespace tactile::editor
