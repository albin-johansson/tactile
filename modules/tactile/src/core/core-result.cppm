// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <utility>

export module tactile.core:result;

import :containers;
import :primitives;
import :strings;

export namespace tactile {

/// Provides common error codes.
enum class Error : u8
{
  /// An unknown error occurred.
  kUnknown,

  /// Not enough memory.
  kOutOfMemory,

  /// A feature isn't supported.
  kUnsupportedFeature,

  /// An invalid operation was attempted.
  kInvalidOp,

  /// An invalid argument was detected.
  kInvalidArg,

  /// An invalid file was detected.
  kInvalidFile,

  /// A file doesn't exist.
  kNoSuchFile,

  /// Requested an out of range value.
  kOutOfRange,

  /// An arithmetic overflow was detected.
  kArithmeticOverflow,

  /// An arithmetic underflow was detected.
  kArithmeticUnderflow,

  /// A stack overflow was detected.
  kStackOverflow,

  /// A stack underflow was detected.
  kStackUnderflow,

  /// Could not parse a file.
  kCouldNotParseFile,

  /// Could not compress data.
  kCouldNotCompress,

  /// Could not decompress data.
  kCouldNotDecompress,
};

template <typename T>
using Result = Expected<T, Error>;

[[nodiscard]]
constexpr auto ok() noexcept -> Result<void>
{
  return Result<void> {};
}

template <typename T>
[[nodiscard]] constexpr auto ok(T&& value) noexcept -> Result<T>
{
  return Result<T> {std::forward<T>(value)};
}

[[nodiscard]]
constexpr auto err(const Error err) noexcept -> Unexpected<Error>
{
  return Unexpected<Error> {err};
}

/// Returns a textual representation of a given error code.
[[nodiscard]]
auto to_string(Error error) -> StringView;

/// Returns a human-readable string that describes a given error code.
[[nodiscard]]
auto describe(Error error) -> StringView;

}  // namespace tactile
