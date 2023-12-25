// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/persist/protobuf.hpp"

#include <google/protobuf/stubs/common.h>

#include "tactile/foundation/log/logger.hpp"

namespace tactile {

Protobuf::Protobuf()
{
  TACTILE_LOG_DEBUG("Initializing Protobuf library");
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

Protobuf::~Protobuf() noexcept
{
  TACTILE_LOG_DEBUG("Shutting down Protobuf library");
  google::protobuf::ShutdownProtobufLibrary();
}

}  // namespace tactile
