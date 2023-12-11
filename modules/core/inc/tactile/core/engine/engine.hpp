// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/engine/engine_app.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/renderer.hpp"

namespace tactile {

/**
 * \brief Provides the main event loop implementation.
 *
 * \see `IEngineApp`
 */
class TACTILE_CORE_API Engine final {
 public:
  /**
   * \brief Creates an engine, but doesn't start it.
   *
   * \param app      the app delegate, must not be null.
   * \param renderer the associated renderer, must not be null.
   */
  explicit Engine(IEngineApp* app, IRenderer* renderer);

  /**
   * \brief Starts the engine loop.
   */
  void run();

 private:
  IEngineApp* mApp;
  IRenderer* mRenderer;
  float mFramebufferScale {0.0f};

  [[nodiscard]]
  auto _poll_events() -> bool;

  void _check_framebuffer_scale();
};

}  // namespace tactile
