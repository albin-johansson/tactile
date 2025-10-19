// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/container/option.hpp"
#include "tactile/core/macros.hpp"
#include "tactile/core/util/defer.hpp"

namespace tactile::editor {

/// RAII utility for SDL library initialization.
class SDLContext final
{
 public:
  TACTILE_DELETE_COPY(SDLContext);
  TACTILE_DELETE_MOVE(SDLContext);

  /// Initializes the SDL library, throwing if unsuccessful.
  SDLContext();

  /// Deinitializes the SDL library.
  ~SDLContext() noexcept = default;

 private:
  Option<Defer<void (*)() noexcept>> m_quitter {};
};

}  // namespace tactile::editor
