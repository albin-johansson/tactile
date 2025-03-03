// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/result.hpp"
#include "tactile/core/fwd.hpp"

namespace tactile {

/// Interface for runtime plugins.
class TACTILE_CORE_API IPlugin
{
 public:
  TACTILE_INTERFACE_CLASS(IPlugin);

  /// Installs the plugin to the given runtime.
  virtual auto install(IRuntime& runtime) -> Result<void> = 0;

  /// Uninstalls the plugin from the given runtime.
  virtual void uninstall(IRuntime& runtime) = 0;
};

}  // namespace tactile
