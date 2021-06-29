#include <GL/glew.h>  // glewInit
#include <gtest/gtest.h>

#include <centurion.hpp>  // library

#include "init_open_gl_attributes.hpp"

auto main(int argc, char** argv) -> int
{
  const cen::library centurion;

  constexpr auto flags = cen::window::default_flags() | cen::window::opengl |
                         cen::window::resizable | cen::window::high_dpi;
  cen::window window{"Tactile tests", cen::window::default_size(), flags};

  cen::gl_context context{window};
  context.make_current(window);

  tactile::InitOpenGLAttributes();

  if (glewInit() != GLEW_OK)
  {
    cen::log::error("Failed to initialize GLEW!");
    return -1;
  }

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}