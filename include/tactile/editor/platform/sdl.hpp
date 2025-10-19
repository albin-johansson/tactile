// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <SDL3/SDL.h>

#include "tactile/core/container/option.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/macros.hpp"
#include "tactile/core/util/defer.hpp"

namespace tactile::editor {

/// RAII utility for SDL library initialization.
class SDLContext final
{
 public:
  TACTILE_DELETE_COPY(SDLContext);
  TACTILE_DELETE_MOVE(SDLContext);

  /// Initializes the SDL library, throwing if unsuccessful.
  SDLContext();

  /// Deinitializes the SDL library.
  ~SDLContext() noexcept = default;

 private:
  Option<Defer<void (*)() noexcept>> m_quitter {};
};

/// Deleter type for SDL_Window.
struct SDLWindowDeleter final
{
  /// Deletes the window via SDL_DestroyWindow.
  static void operator()(SDL_Window* window) noexcept;
};

/// Deleter type for SDL_GPUDevice.
struct SDLGPUDeviceDeleter final
{
  /// Deletes the device via SDL_DestroyGPUDevice.
  static void operator()(SDL_GPUDevice* device) noexcept;
};

/// Alias for a unique SDL window.
using UniqueSDLWindow = Unique<SDL_Window, SDLWindowDeleter>;

/// Alias for a unique SDL GPU device.
using UniqueSDLGPUDevice = Unique<SDL_GPUDevice, SDLGPUDeviceDeleter>;

/// Provides information about an SDL GPU device.
struct SDLGPUDeviceInfo final
{
  /// The GPU device handle.
  UniqueSDLGPUDevice device;

  /// The selected present mode.
  SDL_GPUPresentMode present_mode;

  /// The selected swapchain composition.
  SDL_GPUSwapchainComposition swapchain_composition;
};

/// Creates the SDL window used by the editor.
auto make_editor_window() -> UniqueSDLWindow;

/// Creates the SDL GPU device used by the editor.
auto make_editor_gpu_device(SDL_Window& window, bool debug, const char* driver_name)
    -> SDLGPUDeviceInfo;

}  // namespace tactile::editor
