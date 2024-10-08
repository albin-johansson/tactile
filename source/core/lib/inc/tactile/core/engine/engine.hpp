// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class IEngineApp;
class IRenderer;

/**
 * Provides the main event loop implementation.
 */
class Engine final
{
 public:
  /**
   * Creates an engine, but doesn't start it.
   *
   * \param app      The app delegate, must not be null.
   * \param renderer The associated renderer, must not be null.
   */
  explicit Engine(IEngineApp* app, IRenderer* renderer);

  /**
   * Starts the engine loop.
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
