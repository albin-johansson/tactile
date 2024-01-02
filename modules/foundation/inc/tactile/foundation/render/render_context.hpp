// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/renderer.hpp"
#include "tactile/foundation/render/window.hpp"

namespace tactile {

/**
 * Provides the primary API to access the configured render context resources.
 *
 * \details
 *    This class is used by renderer backends to make their resources available to
 *    the rest of the application.
 */
class TACTILE_FOUNDATION_API RenderContext final {
 public:
  /**
   * Returns the core render context instance.
   *
   * \return
   *    A render context.
   */
  [[nodiscard]]
  static auto get() noexcept -> RenderContext&;

  /**
   * Sets the associated window.
   *
   * \param window The window that will be installed.
   */
  void set_window(IWindow* window) noexcept;

  /**
   * Sets the associated renderer.
   *
   * \param renderer The renderer that will be installed.
   */
  void set_renderer(IRenderer* renderer) noexcept;

  /**
   * Returns the installed window, if any.
   *
   * \return
   *    A window.
   */
  [[nodiscard]]
  auto get_window() noexcept -> IWindow*;

  /**
   * Returns the installed renderer, if any.
   *
   * \return
   *    A renderer.
   */
  [[nodiscard]]
  auto get_renderer() noexcept -> IRenderer*;

 private:
  IWindow* mWindow {};
  IRenderer* mRenderer {};
};

}  // namespace tactile
