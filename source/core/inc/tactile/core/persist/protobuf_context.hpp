// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * RAII type that handles the initialization of the Protobuf library.
 */
class ProtobufContext final {
 public:
  TACTILE_DELETE_COPY(ProtobufContext);
  TACTILE_DELETE_MOVE(ProtobufContext);

  /** Prepares the Protobuf context. */
  [[nodiscard]]
  ProtobufContext();

  /** Cleans up Protobuf resources. */
  ~ProtobufContext() noexcept;
};

}  // namespace tactile