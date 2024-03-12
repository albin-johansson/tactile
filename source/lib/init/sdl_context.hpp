// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/initialization.hpp>
#include <centurion/opengl.hpp>
#include <centurion/window.hpp>

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class SDLContext final {
 public:
  TACTILE_DELETE_COPY(SDLContext);
  TACTILE_DELETE_MOVE(SDLContext);

  [[nodiscard]] SDLContext();

  [[nodiscard]] auto get_window() -> cen::window&;
  [[nodiscard]] auto get_gl_context() -> cen::gl_context&;

 private:
  cen::sdl mSDL;
  cen::img mIMG;
  Maybe<cen::window> mWindow;
  Maybe<cen::gl_context> mGL;
};

}  // namespace tactile
