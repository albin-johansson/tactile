// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class IRuntime;

/**
 * Interface for dynamically loaded modules, aka plugins.
 */
class IPlugin
{
 public:
  TACTILE_INTERFACE_CLASS(IPlugin);

  /**
   * Loads the resources associated with the plugin.
   *
   * \param runtime The associated runtime.
   */
  virtual void load(IRuntime& runtime) = 0;

  /**
   * Releases any resources held by the plugin.
   *
   * \param runtime The associated runtime.
   */
  virtual void unload(IRuntime& runtime) = 0;
};

using PluginConstructor = IPlugin*();
using PluginDestructor = void(IPlugin*);

}  // namespace tactile
