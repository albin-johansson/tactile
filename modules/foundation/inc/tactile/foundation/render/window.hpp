// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Provides the high level window API.
 *
 * \details
 *    Every renderer backend plugin needs to provide an implementation of this
 *    interface, and register an instance of it using the `RenderContext` class.
 */
class IWindow {
 public:
  TACTILE_INTERFACE_CLASS(IWindow);

  /**
   * Makes the window visible.
   */
  virtual void show() = 0;

  /**
   * Makes the window invisible.
   */
  virtual void hide() = 0;

  /**
   * Maximizes the window size within the available region.
   */
  virtual void maximize() = 0;
};

}  // namespace tactile