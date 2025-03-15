// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/string.hpp"
#include "tactile/core/fwd.hpp"

namespace tactile {

/// Interface for controlling and accessing runtime resources.
///
/// Plugins use the functions in this API to make their implementations of core
/// interfaces visible to the editor application. Resources should be registered and
/// unregistered through this API in the IPlugin::install and IPlugin::uninstall
/// functions, respectively. The runtime itself doesn't own any resources, so plugins
/// need to keep their resources alive as long as they're in use.
class IRuntime
{
 public:
  TACTILE_INTERFACE_CLASS(IRuntime);

  /// Installs or uninstalls a renderer.
  virtual void set_renderer(IRenderer* renderer) noexcept = 0;

  /// Returns the registered renderer, if any.
  [[nodiscard]]
  virtual auto get_renderer() noexcept -> IRenderer* = 0;

  /// Installs or uninstalls a compressor with a given identifier.
  virtual void set_compressor(StringView id, ICompressor* compressor) noexcept = 0;

  /// Returns the compressor associated with a given identifier, if any.
  [[nodiscard]]
  virtual auto get_compressor(StringView id) noexcept -> ICompressor* = 0;
};

}  // namespace tactile
