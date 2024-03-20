// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/opengl/api.hpp"
#include "tactile/render/window.hpp"

struct SDL_Window;

namespace tactile {

/**
 * Custom deleter for OpenGL windows.
 */
struct TACTILE_OPENGL_API OpenGLWindowDeleter final {
  void operator()(SDL_Window* window) noexcept;
};

/**
 * Custom deleter for OpenGL contexts.
 */
struct TACTILE_OPENGL_API OpenGLContextDeleter final {
  void operator()(void* context) noexcept;
};

/**
 * Represents an OpenGL window.
 */
class TACTILE_OPENGL_API OpenGLWindow final : public IWindow {
 public:
  TACTILE_DELETE_COPY(OpenGLWindow);
  TACTILE_DEFAULT_MOVE(OpenGLWindow);

  /**
   * Creates an OpenGL window and an associated OpenGL context.
   *
   * \return
   *    An OpenGL window if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto make() -> Result<OpenGLWindow>;

  ~OpenGLWindow() noexcept override = default;

  /**
   * Presents the current framebuffer to the screen buffer.
   */
  void swap_buffers();

  void show() override;

  void hide() override;

  void maximize() override;

  /**
   * Returns the underlying SDL window.
   *
   * \return
   *    An SDL window handle.
   */
  [[nodiscard]]
  auto get_handle() noexcept -> SDL_Window*;

  /**
   * Returns the associated SDL OpenGL context.
   *
   * \return
   *    An opaque OpenGL context.
   */
  [[nodiscard]]
  auto get_context() noexcept -> void*;

 private:
  Unique<SDL_Window, OpenGLWindowDeleter> mWindow;
  Unique<void, OpenGLContextDeleter> mContext;

  OpenGLWindow() = default;
};

}  // namespace tactile
