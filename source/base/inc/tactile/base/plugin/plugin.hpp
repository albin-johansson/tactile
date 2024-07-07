// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Interface for dynamically loaded modules, aka plugins.
 */
class IPlugin
{
 public:
  TACTILE_INTERFACE_CLASS(IPlugin);

  /**
   * Loads the resources associated with the plugin.
   */
  virtual void load() = 0;

  /**
   * Releases any resources held by the plugin.
   */
  virtual void unload() = 0;
};

}  // namespace tactile
