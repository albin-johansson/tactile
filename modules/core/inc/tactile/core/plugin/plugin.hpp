// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \interface IPlugin
 * \brief Interface for dynamically loaded modules, aka plugins.
 */
class IPlugin {
 public:
  TACTILE_INTERFACE_CLASS(IPlugin);

  /** \brief Called when the plugin is loaded. */
  virtual void on_load() = 0;

  /** \brief Called just before the plugin is unloaded. */
  virtual void on_unload() = 0;
};

}  // namespace tactile
