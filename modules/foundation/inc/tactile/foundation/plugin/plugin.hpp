// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \interface IPlugin
 * \brief Interface for dynamically loaded modules, aka plugins.
 */
class IPlugin {
 public:
  TACTILE_INTERFACE_CLASS(IPlugin);

  /**
   * \brief Called when the plugin is loaded.
   */
  virtual void on_load() = 0;

  /**
   * \brief Called just before the plugin is unloaded.
   */
  virtual void on_unload() noexcept = 0;
};

}  // namespace tactile