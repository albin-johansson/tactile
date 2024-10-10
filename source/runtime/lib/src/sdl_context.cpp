// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/runtime/sdl_context.hpp"

#include <SDL2/SDL.h>

#include "tactile/core/debug/exception.hpp"
#include "tactile/core/log/logger.hpp"

namespace tactile::runtime {

SDLContext::SDLContext()
{
  TACTILE_LOG_TRACE("Initializing SDL library");

  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
    TACTILE_LOG_FATAL("Could not initialize SDL: {}", SDL_GetError());
    throw core::Exception {"Could not initialize SDL"};
  }

  SDL_SetHint(SDL_HINT_APP_NAME, "Tactile");
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

SDLContext::~SDLContext() noexcept
{
  SDL_Quit();
}

}  // namespace tactile::runtime
