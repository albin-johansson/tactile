// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/render/window.hpp"
#include "tactile/opengl/api.hpp"

struct SDL_Window;

namespace tactile::gl {

/**
 * \brief Deleter type for SDL windows.
 */
struct TACTILE_OPENGL_API SDLWindowDeleter final {
  void operator()(SDL_Window* window) noexcept;
};

/**
 * \brief Deleter type for OpenGL contexts.
 */
struct TACTILE_OPENGL_API OpenGLContextDeleter final {
  void operator()(void* context) noexcept;
};

/**
 * \brief An OpenGL 4 window implementation.
 */
class TACTILE_OPENGL_API OpenGLWindow final : public IWindow {
 public:
  /**
   * \brief Creates an OpenGL window and an associated OpenGL context.
   */
  OpenGLWindow();

  /**
   * \brief Swaps the active framebuffer.
   */
  void swap_framebuffer();

  void show() override;

  void hide() override;

  void maximize() override;

  /**
   * \brief Returns the underlying window.
   *
   * \return a window handle.
   */
  [[nodiscard]]
  auto get_handle() -> SDL_Window*;

 private:
  Unique<SDL_Window, SDLWindowDeleter> mWindow;
  Unique<void, OpenGLContextDeleter> mContext;
};

}  // namespace tactile::gl
