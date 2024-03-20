// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_window.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "tactile/opengl/opengl_error.hpp"

namespace tactile {

void OpenGLWindowDeleter::operator()(SDL_Window* window) noexcept
{
  SDL_DestroyWindow(window);
}

void OpenGLContextDeleter::operator()(void* context) noexcept
{
  SDL_GL_DeleteContext(context);
}

auto OpenGLWindow::make() -> Result<OpenGLWindow>
{
  OpenGLWindow window {};

  const auto window_flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE |
                            SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
  window.mWindow.reset(SDL_CreateWindow("Tactile " TACTILE_VERSION_STRING,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        800,
                                        600,
                                        window_flags));
  if (!window.mWindow) {
    return unexpected(make_error(OpenGLError::kWindowError));
  }

  window.mContext.reset(SDL_GL_CreateContext(window.mWindow.get()));
  if (!window.mContext) {
    return unexpected(make_error(OpenGLError::kContextError));
  }

  SDL_GL_SetSwapInterval(1);

  return window;
}

void OpenGLWindow::swap_buffers()
{
  if constexpr (kOnMacos) {
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

void OpenGLWindow::maximize()
{
  SDL_MaximizeWindow(mWindow.get());
}

auto OpenGLWindow::get_handle() noexcept -> SDL_Window*
{
  return mWindow.get();
}

auto OpenGLWindow::get_context() noexcept -> void*
{
  return mContext.get();
}

}  // namespace tactile
