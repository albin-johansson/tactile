#include <GL/glew.h>  // glewInit
#include <gtest/gtest.h>

#include <centurion.hpp>  // library
#include <cstdlib>        // atexit

#include "gl_attributes.hpp"
#include "io/preferences.hpp"
#include "utils/load_texture.hpp"

auto main(int argc, char** argv) -> int
{
  std::atexit(Tactile::UnloadTextures);

  const cen::library centurion;

  constexpr auto flags = cen::window::default_flags() | cen::window::opengl |
                         cen::window::resizable | cen::window::high_dpi;
  cen::window window{"Tactile tests", cen::window::default_size(), flags};

  cen::gl_context context{window};
  context.make_current(window);

  Tactile::InitOpenGLAttributes();

  if (glewInit() != GLEW_OK) {
    cen::log::error("Failed to initialize GLEW!");
    return -1;
  }

  Tactile::LoadPreferences();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}