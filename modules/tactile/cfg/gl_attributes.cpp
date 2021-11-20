#include "gl_attributes.hpp"

#include <centurion.hpp>

namespace Tactile {
namespace {

constexpr int major_version = 3;
constexpr int minor_version = 0;

}  // namespace

void InitOpenGLAttributes()
{
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  if constexpr (cen::ifdef_apple()) {
    cen::gl::set(cen::gl_attribute::flags, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    cen::gl::set(cen::gl_attribute::context_profile_mask, SDL_GL_CONTEXT_PROFILE_CORE);
    cen::gl::set(cen::gl_attribute::context_major_version, 3);
    cen::gl::set(cen::gl_attribute::context_minor_version, 2);
  }
  else {
    cen::gl::set(cen::gl_attribute::context_profile_mask, SDL_GL_CONTEXT_PROFILE_CORE);
    cen::gl::set(cen::gl_attribute::context_major_version, 3);
    cen::gl::set(cen::gl_attribute::context_minor_version, 0);
  }

  cen::gl::set(cen::gl_attribute::double_buffer, 1);
  cen::gl::set(cen::gl_attribute::depth_size, 24);
  cen::gl::set(cen::gl_attribute::stencil_size, 8);
}

}  // namespace Tactile
