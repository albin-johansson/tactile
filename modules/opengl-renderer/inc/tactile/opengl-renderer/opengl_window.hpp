// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/window.hpp"
#include "tactile/opengl-renderer/api.hpp"

struct SDL_Window;

namespace tactile::opengl {

/**
 * Deleter type for SDL windows.
 */
struct TACTILE_OPENGL_API SDLWindowDeleter final {
  void operator()(SDL_Window* window) noexcept;
};

/**
 * Deleter type for OpenGL contexts.
 */
struct TACTILE_OPENGL_API OpenGLContextDeleter final {
  void operator()(void* context) noexcept;
};

/**
 * An OpenGL 4 window implementation.
 */
class TACTILE_OPENGL_API OpenGLWindow final : public IWindow {
 public:
  using ManagedWindow = Unique<SDL_Window, SDLWindowDeleter>;
  using ManagedContext = Unique<void, OpenGLContextDeleter>;

  /**
   * Creates an OpenGL window and an associated OpenGL context.
   *
   * \return
   *    The created window.
   */
  [[nodiscard]]
  static auto create() -> Result<OpenGLWindow>;

  /**
   * Swaps the active framebuffer.
   */
  void swap_framebuffer();

  void show() override;

  void hide() override;

  void maximize() override;

  /**
   * Returns the underlying window.
   *
   * \return
   *    A window handle.
   */
  [[nodiscard]]
  auto get_handle() -> SDL_Window*;

 private:
  ManagedWindow mWindow;
  ManagedContext mContext;

  OpenGLWindow(ManagedWindow window, ManagedContext context);
};

}  // namespace tactile::opengl