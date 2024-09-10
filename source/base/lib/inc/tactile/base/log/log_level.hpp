// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * The supported log level categories.
 *
 * \details
 * The underlying value increases with severity.
 */
enum class LogLevel : std::uint8_t
{
  kTrace,  ///< For verbose messages only useful for detailed debugging.
  kDebug,  ///< For messages that provide useful developer information.
  kInfo,   ///< For messages that provide general high-level information.
  kWarn,   ///< For messages that warn about potential issues.
  kError,  ///< For messages that report non-fatal errors.
  kFatal,  ///< For reporting errors from which the application cannot recover.
};

}  // namespace tactile
