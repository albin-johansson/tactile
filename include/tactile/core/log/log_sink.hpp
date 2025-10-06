// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/log/log_entry.hpp"
#include "tactile/core/macros.hpp"

namespace tactile {

/// Interface for objects that handle prepared messages from an associated logger.
class ILogSink
{
 public:
  TACTILE_INTERFACE(ILogSink);

  /// Flushes any pending messages.
  virtual void flush() = 0;

  /// Logs a given log entry.
  virtual void log(const LogEntry& entry) = 0;
};

}  // namespace tactile
