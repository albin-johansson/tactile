// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Handles the loading and unloading of the Google Protobuf context.
 */
class TACTILE_CORE_API Protobuf final {
 public:
  TACTILE_DELETE_COPY(Protobuf);
  TACTILE_DELETE_MOVE(Protobuf);

  [[nodiscard]] Protobuf();

  ~Protobuf() noexcept;
};

}  // namespace tactile
