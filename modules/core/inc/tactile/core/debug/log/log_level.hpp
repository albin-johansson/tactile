// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

namespace tactile {

/** The log level categories. The underlying value increases with severity. */
enum class LogLevel : uint8 {
  kTrace = 0,
  kDebug = 1,
  kInfo = 2,
  kWarn = 3,
  kError = 4,
  kFatal = 5,
};

}  // namespace tactile
