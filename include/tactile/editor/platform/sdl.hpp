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

/// Alias for a unique SDL window.
using UniqueSDLWindow = Unique<SDL_Window, SDLWindowDeleter>;

/// Creates the SDL window used by the editor.
auto make_editor_window() -> UniqueSDLWindow;

}  // namespace tactile::editor
