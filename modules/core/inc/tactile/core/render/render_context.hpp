// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/render/renderer.hpp"
#include "tactile/core/render/window.hpp"

namespace tactile {

/**
 * \brief Provides the primary API to access the configured render context resources.
 *
 * \details This class is used by renderer backends to make their resources available to
 *          the rest of the application.
 */
class TACTILE_CORE_API RenderContext final {
 public:
  /**
   * \brief Returns the core render context instance.
   *
   * \return a render context.
   */
  [[nodiscard]]
  static auto get() noexcept -> RenderContext&;

  /**
   * \brief Sets the associated window.
   *
   * \param window the window that will be installed.
   */
  void set_window(IWindow* window) noexcept;

  /**
   * \brief Sets the associated renderer.
   *
   * \param renderer the renderer that will be installed.
   */
  void set_renderer(IRenderer* renderer) noexcept;

  /**
   * \brief Returns the installed window, if any.
   *
   * \return a window.
   */
  [[nodiscard]]
  auto get_window() noexcept -> IWindow*;

  /**
   * \brief Returns the installed renderer, if any.
   *
   * \return a renderer.
   */
  [[nodiscard]]
  auto get_renderer() noexcept -> IRenderer*;

 private:
  IWindow* mWindow {};
  IRenderer* mRenderer {};
};

}  // namespace tactile
