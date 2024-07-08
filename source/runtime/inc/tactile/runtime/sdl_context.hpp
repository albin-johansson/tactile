// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/runtime/api.hpp"

namespace tactile {

/**
 * RAII type that manages the initialization of the SDL library.
 */
class TACTILE_RUNTIME_API SDLContext final
{
 public:
  TACTILE_DELETE_COPY(SDLContext);
  TACTILE_DELETE_MOVE(SDLContext);

  [[nodiscard]]
  SDLContext();

  ~SDLContext() noexcept;
};

}  // namespace tactile
