// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/compression/compression_error.hpp"

#include <utility>  // to_underlying

namespace tactile {
namespace {

inline constexpr CompressionErrorDomain kCompressionErrorDomain;

}  // namespace

auto CompressionErrorDomain::get_message(const uint32 error_id) const noexcept
    -> StringView
{
  switch (Error {error_id}) {
    case Error::kNoData: return "No data to compress/decompress";
    case Error::kInternalError: return "An internal error occurred";
    case Error::kInvalidMode: return "An invalid compression mode was detected";
  }

  return "";
}

auto error(const CompressionError error) noexcept -> Unexpected<ErrorCode>
{
  return unexpected(ErrorCode {&kCompressionErrorDomain, std::to_underlying(error)});
}

}  // namespace tactile