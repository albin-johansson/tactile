// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_window.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "tactile/core/debug/error.hpp"
#include "tactile/core/debug/log/logger.hpp"

namespace tactile::gl {

void SDLWindowDeleter::operator()(SDL_Window* window) noexcept
{
  SDL_DestroyWindow(window);
}

void OpenGLContextDeleter::operator()(void* context) noexcept
{
  SDL_GL_DeleteContext(context);
}

OpenGLWindow::OpenGLWindow()
  : mWindow {SDL_CreateWindow("Tactile " TACTILE_VERSION_STRING,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800,
                              600,
                              SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE |
                                  SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL)}
{
  if (!mWindow) {
    TACTILE_LOG_FATAL("Could not create OpenGL window: {}", SDL_GetError());
    throw Error {"Could not create OpenGL window"};
  }

  mContext.reset(SDL_GL_CreateContext(mWindow.get()));
  if (!mContext) {
    TACTILE_LOG_FATAL("Could not create OpenGL context: {}", SDL_GetError());
    throw Error {"Could not create OpenGL context"};
  }

  // Enable VSync.
  SDL_GL_SetSwapInterval(1);
}

void OpenGLWindow::swap_framebuffer()
{
  if constexpr (kIsApple) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }
  SDL_GL_SwapWindow(mWindow.get());
}

void OpenGLWindow::show()
{
  SDL_ShowWindow(mWindow.get());
}

void OpenGLWindow::hide()
{
  SDL_HideWindow(mWindow.get());
}

auto OpenGLWindow::get_handle() -> SDL_Window*
{
  return mWindow.get();
}

}  // namespace tactile::gl
