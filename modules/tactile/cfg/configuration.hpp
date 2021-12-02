#pragma once

#include <tactile_def.hpp>

#include <centurion.hpp>

#include "editor/gui/imgui_context.hpp"
#include "protobuf_context.hpp"

namespace Tactile {

/* This class handles the configuration of aspects such as windows and OpenGL contexts */
class ApplicationConfiguration final
{
 public:
  ApplicationConfiguration();

  /* Returns the application window, don't free the returned pointer! */
  [[nodiscard]] auto GetWindow() -> SDL_Window*;

 private:
  ProtobufContext mProtobuf;
  cen::library mCenturion;

  /* Initialization of these members need to be deferred */
  Maybe<cen::window> mWindow;
  Maybe<cen::gl_context> mOpenGL;
  Maybe<ImGuiContext> mImGui;
};

}  // namespace Tactile
