// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Interface for applications that can may be injected into an \c Engine
 * instance.
 */
class IEngineApp
{
 public:
  TACTILE_INTERFACE_CLASS(IEngineApp);

  /**
   * Called when the app starts running.
   */
  virtual void on_startup() = 0;

  /**
   * Called when the app stops running.
   */
  virtual void on_shutdown() = 0;

  /**
   * Called once per render frame.
   */
  virtual void on_update() = 0;

  /**
   * Called whenever the display framebuffer scale changes.
   *
   * \note
   * This function is always called outside of an active frame.
   *
   * \param framebuffer_scale The new framebuffer scale.
   */
  virtual void on_framebuffer_scale_changed(float framebuffer_scale) = 0;
};

}  // namespace tactile
