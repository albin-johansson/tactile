// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/** Interface for dynamically loaded modules, aka plugins. */
class TACTILE_CORE_API IPlugin {
 public:
  TACTILE_INTERFACE_CLASS(IPlugin);

  /** Called when the plugin is loaded. */
  virtual void on_load() = 0;

  /** Called just before the plugin is unloaded. */
  virtual void on_unload() = 0;
};

}  // namespace tactile