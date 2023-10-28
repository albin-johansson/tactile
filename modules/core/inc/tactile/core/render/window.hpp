// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \interface IWindow
 * \brief Provides the high level window API.
 *
 * \details Every renderer backend plugin needs to provide an implementation of this
 *          interface, and register an instance of it using the `RenderContext` class.
 */
class TACTILE_CORE_API IWindow {
 public:
  TACTILE_INTERFACE_CLASS(IWindow);

  /**
   * \brief Makes the window visible.
   */
  virtual void show() = 0;

  /**
   * \brief Makes the window invisible.
   */
  virtual void hide() = 0;
};

}  // namespace tactile