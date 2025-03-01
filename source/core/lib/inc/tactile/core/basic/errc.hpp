// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/primitives.hpp"
#include "tactile/core/basic/string.hpp"

namespace tactile {

/// Provides common error codes.
enum class Errc : u8
{
  /// An unknown error occurred.
  kUnknown,

  /// An operation or feature isn't supported.
  kNotSupported,

  /// Not enough memory.
  kOutOfMemory,

  /// A stack overflow was detected.
  kStackOverflow,

  /// A stack underflow was detected.
  kStackUnderflow,

  /// Initialization failed.
  kBadInit,

  /// A given parameter is invalid.
  kBadParam,

  /// An invalid state was detected.
  kBadState,

  /// An invalid operation was attempted.
  kBadOperation,

  /// A file doesn't exist.
  kNoSuchFile,

  /// A file error occurred.
  kBadFile,

  /// A file stream couldn't be created.
  kBadFileStream,

  /// A file couldn't be copied.
  kBadFileCopy,

  /// An invalid image was detected.
  kBadImage,

  /// A parse operation failed.
  kParseError,

  /// A write operation failed.
  kWriteError,

  /// A compression operation failed.
  kCouldNotCompress,

  /// A decompression operation failed.
  kCouldNotDecompress,
};

[[nodiscard]]
auto to_string(Errc errc) noexcept -> StringView;

}  // namespace tactile
