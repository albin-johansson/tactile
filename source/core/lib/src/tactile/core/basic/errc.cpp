// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/basic/errc.hpp"

namespace tactile {

auto to_string(const Errc errc) noexcept -> StringView
{
  switch (errc) {
    case Errc::kUnknown:            return "unknown";
    case Errc::kNotSupported:       return "not supported";
    case Errc::kOutOfMemory:        return "out of memory";
    case Errc::kStackOverflow:      return "stack overflow";
    case Errc::kStackUnderflow:     return "stack underflow";
    case Errc::kBadInit:            return "initialization error";
    case Errc::kBadParam:           return "invalid parameter";
    case Errc::kBadState:           return "invalid state";
    case Errc::kBadOperation:       return "invalid operation";
    case Errc::kNoSuchFile:         return "no such file";
    case Errc::kBadFileStream:      return "file stream error";
    case Errc::kBadFileCopy:        return "file copy error";
    case Errc::kBadImage:           return "invalid image";
    case Errc::kParseError:         return "parse error";
    case Errc::kWriteError:         return "write error";
    case Errc::kCouldNotCompress:   return "could not compress";
    case Errc::kCouldNotDecompress: return "could not decompress";
  }

  return "?";
}

}  // namespace tactile
