#include <GL/glew.h>  // glewInit

#include <centurion.hpp>  // library, window, gl_context
#include <utility>        // move

#include "application.hpp"
#include "imgui_wrapper.hpp"
#include "init_open_gl_attributes.hpp"

auto main(int argc, char** argv) -> int
{
  cen::library centurion;

  tactile::InitOpenGLAttributes();

  constexpr auto flags = cen::window::default_flags() | cen::window::opengl |
                         cen::window::resizable | cen::window::high_dpi;
  cen::window window{"Tactile", cen::window::default_size(), flags};

  cen::gl_context context{window};
  context.make_current(window);

  if (glewInit() != GLEW_OK)
  {
    cen::log::error("Failed to initialize GLEW!");
    return -1;
  }

  tactile::ImGuiWrapper wrapper{window, context};
  window.maximize();

  tactile::Application application{std::move(window), std::move(context)};
  return application.Run();
}