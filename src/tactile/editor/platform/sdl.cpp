// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/platform/sdl.hpp"

#include <stdexcept>

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

}  // namespace tactile::editor
