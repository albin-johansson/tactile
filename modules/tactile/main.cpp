#include <utility>  // move

#include <GL/glew.h>      // glewInit
#include <centurion.hpp>  // library, window, gl_context

#include "application.hpp"
#include "cfg/gl_attributes.hpp"
#include "cfg/logging.hpp"
#include "cfg/protobuf_context.hpp"
#include "editor/gui/imgui_context.hpp"
#include "io/preferences.hpp"

auto main(int, char**) -> int
{
  Tactile::ProtobufContext protobuf;

  cen::library centurion;

  SDL_LogSetOutputFunction(
      [](void* /*data*/,
         int /*category*/,
         const SDL_LogPriority priority,
         const char* msg) {
        Tactile::LogMessage(static_cast<cen::log_priority>(priority), msg);
      },
      nullptr);

  if constexpr (cen::is_debug_build()) {
    cen::log::set_priority(cen::log_priority::verbose);
  }
  else {
    cen::log::set_priority(cen::log_priority::info);
  }

  constexpr auto flags =
      cen::window::hidden | cen::window::opengl | cen::window::resizable;
  cen::window window{"Tactile", cen::window::default_size(), flags};
  window.set_icon(cen::surface{"resources/icon.png"});

  cen::gl_context glContext{window};
  glContext.make_current(window);

  Tactile::InitOpenGLAttributes();

  if (glewInit() != GLEW_OK) {
    cen::log::error("Failed to initialize GLEW!");
    return -1;
  }

  Tactile::LoadPreferences();

  Tactile::ImGuiContext gui{window, glContext};
  window.maximize();

  Tactile::Application app{std::move(window)};
  return app.Run();
}