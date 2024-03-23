// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * RAII type that manages the initialization of the SDL library.
 */
class SDLContext final {
 public:
  TACTILE_DELETE_COPY(SDLContext);
  TACTILE_DELETE_MOVE(SDLContext);

  [[nodiscard]]
  SDLContext();

  ~SDLContext() noexcept;
};

}  // namespace tactile
