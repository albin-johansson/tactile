// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/container/string.hpp"
#include "tactile/core/log/log_category.hpp"

namespace tactile {

/// A view of a prepared message from a logger.
struct LogEntry final
{
  /// The message category.
  LogCategory category {};

  /// The message prefix, including the category and timestamp.
  StringView prefix {};

  /// The logged message.
  StringView message {};
};

}  // namespace tactile
