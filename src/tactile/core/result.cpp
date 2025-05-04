// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module tactile.core;

namespace tactile {

auto to_string(const Error error) -> StringView
{
  switch (error) {
    case Error::kUnknown:                return "unknown";
    case Error::kOutOfMemory:            return "out_of_memory";
    case Error::kUnsupportedFeature:     return "unsupported_feature";
    case Error::kInvalidOp:              return "invalid_op";
    case Error::kInvalidArg:             return "invalid_arg";
    case Error::kInvalidFile:            return "invalid_file";
    case Error::kNoSuchFile:             return "no_such_file";
    case Error::kOutOfRange:             return "out_of_range";
    case Error::kArithmeticOverflow:     return "arithmetic_overflow";
    case Error::kArithmeticUnderflow:    return "arithmetic_underflow";
    case Error::kArithmeticPrecision:    return "arithmetic_precision";
    case Error::kArithmeticInvalidValue: return "arithmetic_invalid_value";
    case Error::kStackOverflow:          return "stack_overflow";
    case Error::kStackUnderflow:         return "stack_underflow";
    case Error::kCouldNotParseFile:      return "could_not_parse_file";
    case Error::kCouldNotCompress:       return "could_not_compress";
    case Error::kCouldNotDecompress:     return "could_not_decompress";
  }

  return "?";
}

auto describe(const Error error) -> StringView
{
  switch (error) {
    case Error::kUnknown:                return "an unknown error occurred";
    case Error::kOutOfMemory:            return "out of memory";
    case Error::kUnsupportedFeature:     return "a feature isn't supported";
    case Error::kInvalidOp:              return "attempted an invalid operation";
    case Error::kInvalidArg:             return "detected an invalid argument";
    case Error::kInvalidFile:            return "detected an invalid file";
    case Error::kNoSuchFile:             return "an expected file didn't exist";
    case Error::kOutOfRange:             return "requested an out of range value";
    case Error::kArithmeticOverflow:     return "detected arithmetic overflow";
    case Error::kArithmeticUnderflow:    return "detected arithmetic underflow";
    case Error::kArithmeticPrecision:    return "detected loss of arithmetic precision";
    case Error::kArithmeticInvalidValue: return "detected invalid arithmetic value";
    case Error::kStackOverflow:          return "detected stack overflow";
    case Error::kStackUnderflow:         return "detected stack underflow";
    case Error::kCouldNotParseFile:      return "could not parse a file";
    case Error::kCouldNotCompress:       return "could not compress data";
    case Error::kCouldNotDecompress:     return "could not decompress data";
  }

  return "?";
}

}  // namespace tactile
