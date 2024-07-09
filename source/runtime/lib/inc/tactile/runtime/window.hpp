// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/render/window.hpp"
#include "tactile/runtime/api.hpp"

struct SDL_Window;

namespace tactile {

/**
 * Custom deleter for an SDL window.
 */
struct TACTILE_RUNTIME_API WindowHandleDeleter final
{
  void operator()(SDL_Window* window) noexcept;
};

/**
 * Represents an operating system window.
 */
class TACTILE_RUNTIME_API Window final : public IWindow
{
 public:
  TACTILE_DELETE_COPY(Window);
  TACTILE_DEFAULT_MOVE(Window);

  /**
   * Creates a window.
   *
   * \details
   * No renderer API will be specified by default, so the appropriate flag for
   * the chosen renderer needs to be passed to this function. For example,
   * \c SDL_WINDOW_OPENGL for OpenGL based windows.
   *
   * \param extra_flags Additional window flags used during construction.
   *
   * \return
   * A window if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto create(uint32 extra_flags) -> Result<Window>;

  ~Window() noexcept override = default;

  void show() override;

  void hide() override;

  void maximize() override;

  [[nodiscard]]
  auto get_handle() -> SDL_Window* override;

 private:
  Unique<SDL_Window, WindowHandleDeleter> mWindow {};

  explicit Window(SDL_Window* window);
};

}  // namespace tactile
