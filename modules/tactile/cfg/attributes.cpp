#include "attributes.hpp"

#include <SDL.h>

#include "build.hpp"

namespace tactile {

void InitAttributes()
{
  /* Ensure nearest pixel sampling */
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

  /* Enable multi-gesture events from touchpads */
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

  /* Make sure that we use OpenGL */
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  if constexpr (IsPlatformOSX()) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
}

}  // namespace tactile
