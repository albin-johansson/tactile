// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/// Handles the loading and unloading of the Protobuf library.
class ProtobufContext final {
 public:
  TACTILE_DELETE_COPY(ProtobufContext);
  TACTILE_DELETE_MOVE(ProtobufContext);

  [[nodiscard]] ProtobufContext();

  ~ProtobufContext();
};

}  // namespace tactile
