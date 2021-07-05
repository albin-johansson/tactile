#include "init_open_gl_attributes.hpp"

#include <centurion.hpp>

namespace Tactile {
namespace {

inline constexpr int major_version = 3;
inline constexpr int minor_version = 0;

}  // namespace

void InitOpenGLAttributes()
{
  cen::gl::set(cen::gl_attribute::flags, 0);
  cen::gl::set(cen::gl_attribute::context_profile_mask, SDL_GL_CONTEXT_PROFILE_CORE);
  cen::gl::set(cen::gl_attribute::context_major_version, major_version);
  cen::gl::set(cen::gl_attribute::context_minor_version, minor_version);

  cen::gl::set(cen::gl_attribute::double_buffer, 1);
  cen::gl::set(cen::gl_attribute::depth_size, 24);
  cen::gl::set(cen::gl_attribute::stencil_size, 8);

  // Enable VSync
  cen::gl::set_swap_interval(cen::gl_swap_interval::synchronized);
}

}  // namespace Tactile
