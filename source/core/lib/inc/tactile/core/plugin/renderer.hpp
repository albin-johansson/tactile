// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/result.hpp"

union SDL_Event;

namespace tactile {

/// Interface implemented by renderer plugins.
class TACTILE_CORE_API IRenderer
{
 public:
  TACTILE_INTERFACE_CLASS(IRenderer);

  /// Attempts to begin a new render frame.
  [[nodiscard]]
  virtual auto begin_frame() -> Result<void> = 0;

  /// Ends the current render frame started by `begin_frame`.
  virtual void end_frame() = 0;

  /// Reloads the font assets, if possible.
  virtual void reload_fonts() = 0;

  /// Processes a single event.
  virtual void process_event(const SDL_Event& event) = 0;
};

}  // namespace tactile
