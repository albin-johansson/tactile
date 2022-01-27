#include "sdl_attributes.hpp"

#include <SDL.h>
#include <centurion.hpp>

#include "build.hpp"

namespace tactile {

void init_sdl_attributes()
{
  /* Ensure nearest pixel sampling */
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

  /* Enable multi-gesture events from touchpads */
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

  /* Make sure that we use OpenGL */
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  if constexpr (on_osx) {
    cen::gl::set(cen::gl_attribute::context_flags,
                 SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  cen::gl::set(cen::gl_attribute::context_profile_mask, SDL_GL_CONTEXT_PROFILE_CORE);
  cen::gl::set(cen::gl_attribute::context_major_version, 3);
  cen::gl::set(cen::gl_attribute::context_minor_version, 2);

  cen::gl::set(cen::gl_attribute::double_buffer, 1);
}

}  // namespace tactile
