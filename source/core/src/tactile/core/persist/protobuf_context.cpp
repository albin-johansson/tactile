// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/persist/protobuf_context.hpp"

#include <google/protobuf/stubs/common.h>

#include "tactile/core/log/logger.hpp"

namespace tactile {

ProtobufContext::ProtobufContext()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

ProtobufContext::~ProtobufContext() noexcept
{
  try {
    google::protobuf::ShutdownProtobufLibrary();
  }
  catch (...) {
    TACTILE_LOG_ERROR("Protobuf shutdown failed");
  }
}

}  // namespace tactile
