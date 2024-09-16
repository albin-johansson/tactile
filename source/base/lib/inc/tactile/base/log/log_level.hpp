// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t
#include <format>   // formatter, format_to

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

template <>
struct std::formatter<tactile::LogLevel> final
{
  template <typename FormatParseContext>
  constexpr static auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::LogLevel& level, FormatContext& ctx) const
  {
    switch (level) {
      case tactile::LogLevel::kTrace: return std::format_to(ctx.out(), "trace");
      case tactile::LogLevel::kDebug: return std::format_to(ctx.out(), "debug");
      case tactile::LogLevel::kInfo:  return std::format_to(ctx.out(), "info");
      case tactile::LogLevel::kWarn:  return std::format_to(ctx.out(), "warn");
      case tactile::LogLevel::kError: return std::format_to(ctx.out(), "error");
      case tactile::LogLevel::kFatal: return std::format_to(ctx.out(), "fatal");
      default:                        return std::format_to(ctx.out(), "?");
    }
  }
};
