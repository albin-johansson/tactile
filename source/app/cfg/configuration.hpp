#pragma once

#include <centurion.hpp>

#include "imgui_context.hpp"
#include "protobuf_context.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Handles the configuration of the application window, OpenGL context, etc.
 *
 * \ingroup cfg
 */
class app_configuration final
{
 public:
  app_configuration();

  /* Returns the application window */
  [[nodiscard]] auto window() -> cen::window&;

 private:
  protobuf_context mProtobuf;
  cen::sdl mSDL;
  cen::img mIMG;

  /* Initialization of these members needs to be deferred */
  maybe<cen::window> mWindow;
  maybe<cen::gl_context> mOpenGL;
  maybe<im_gui_context> mImGui;
};

}  // namespace tactile
