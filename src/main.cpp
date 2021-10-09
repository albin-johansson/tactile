#include <GL/glew.h>  // glewInit
#include <google/protobuf/stubs/common.h>

#include <centurion.hpp>  // library, window, gl_context
#include <utility>        // move

#include "application.hpp"
#include "gl_attributes.hpp"
#include "gui/imgui_context.hpp"
#include "io/preferences.hpp"

namespace Tactile {

struct ProtobufContext final
{
  ProtobufContext()
  {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
  }

  ~ProtobufContext()
  {
    google::protobuf::ShutdownProtobufLibrary();
  }
};

}  // namespace Tactile

auto main(int, char**) -> int
{
  Tactile::ProtobufContext protobuf;

  cen::library centurion;
  cen::log::use_preset_output_function();

  if constexpr (cen::is_debug_build()) {
    cen::log::set_priority(cen::log_priority::debug);
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