// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/platform/sdl.hpp"

#include <stdexcept>

#include <SDL3/SDL.h>

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

}  // namespace tactile::editor
