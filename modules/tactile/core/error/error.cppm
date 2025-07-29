// Copyright (C) 2025 Albin Johansson

export module tactile.core.error;

import tactile.core.ext.std;
import tactile.core.prelude;

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

  /// An arithmetic precision error was detected.
  kArithmeticPrecision,

  /// An invalid arithmetic value was detected.
  kArithmeticInvalidValue,

  /// A stack overflow was detected.
  kStackOverflow,

  /// A stack underflow was detected.
  kStackUnderflow,

  /// An initialization error occurred.
  kCouldNotInitialize,

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
