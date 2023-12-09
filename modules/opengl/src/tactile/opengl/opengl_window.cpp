// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_window.hpp"

#include <utility>  // move

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::gl {

void SDLWindowDeleter::operator()(SDL_Window* window) noexcept
{
  SDL_DestroyWindow(window);
}

void OpenGLContextDeleter::operator()(void* context) noexcept
{
  SDL_GL_DeleteContext(context);
}

auto OpenGLWindow::create() -> Result<OpenGLWindow>
{
  ManagedWindow window {SDL_CreateWindow("Tactile " TACTILE_VERSION_STRING,
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         800,
                                         600,
                                         SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE |
                                             SDL_WINDOW_ALLOW_HIGHDPI |
                                             SDL_WINDOW_OPENGL)};

  if (!window) {
    TACTILE_LOG_ERROR("Could not create OpenGL window: {}", SDL_GetError());
    return unexpected(make_generic_error(GenericError::kInitFailure));
  }

  ManagedContext context {SDL_GL_CreateContext(window.get())};

  if (!context) {
    TACTILE_LOG_ERROR("Could not create OpenGL context: {}", SDL_GetError());
    return unexpected(make_generic_error(GenericError::kInitFailure));
  }

  return OpenGLWindow {std::move(window), std::move(context)};
}

OpenGLWindow::OpenGLWindow(ManagedWindow window, ManagedContext context)
  : mWindow {std::move(window)},
    mContext {std::move(context)}
{
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

void OpenGLWindow::maximize()
{
  SDL_MaximizeWindow(mWindow.get());
}

auto OpenGLWindow::get_handle() -> SDL_Window*
{
  return mWindow.get();
}

}  // namespace tactile::gl
