// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string_view>  // string_view

namespace tactile {

/**
 * Provides common error codes.
 */
enum class ErrorCode : int
{
  /** An unknown error occurred. */
  kUnknown,

  /** An operation or feature isn't supported. */
  kNotSupported,

  /** Not enough memory. */
  kOutOfMemory,

  /** A stack overflow was detected. */
  kStackOverflow,

  /** A stack underflow was detected. */
  kStackUnderflow,

  /** Initialization failed. */
  kBadInit,

  /** A given parameter is invalid. */
  kBadParam,

  /** An invalid state was detected. */
  kBadState,

  /** An invalid operation was attempted. */
  kBadOperation,

  /** A file doesn't exist. */
  kNoSuchFile,

  /** A file stream couldn't be created. */
  kBadFileStream,

  /** A file couldn't be copied. */
  kBadFileCopy,

  /** An invalid image was detected. */
  kBadImage,

  /** An invalid save file was detected. */
  kBadSaveFile,

  /** A compression operation failed. */
  kCouldNotCompress,

  /** A decompression operation failed. */
  kCouldNotDecompress,
};

[[nodiscard]]
constexpr auto to_string(const ErrorCode errc) noexcept -> std::string_view
{
  switch (errc) {
    case ErrorCode::kUnknown:            return "unknown";
    case ErrorCode::kNotSupported:       return "not supported";
    case ErrorCode::kOutOfMemory:        return "out of memory";
    case ErrorCode::kStackOverflow:      return "stack overflow";
    case ErrorCode::kStackUnderflow:     return "stack underflow";
    case ErrorCode::kBadInit:            return "initialization error";
    case ErrorCode::kBadParam:           return "invalid parameter";
    case ErrorCode::kBadState:           return "invalid state";
    case ErrorCode::kBadOperation:       return "invalid operation";
    case ErrorCode::kNoSuchFile:         return "no such file";
    case ErrorCode::kBadFileStream:      return "file stream error";
    case ErrorCode::kBadFileCopy:        return "file copy error";
    case ErrorCode::kBadImage:           return "invalid image";
    case ErrorCode::kBadSaveFile:        return "invalid save file";
    case ErrorCode::kCouldNotCompress:   return "could not compress";
    case ErrorCode::kCouldNotDecompress: return "could not decompress";
  }

  return "?";
}

}  // namespace tactile
