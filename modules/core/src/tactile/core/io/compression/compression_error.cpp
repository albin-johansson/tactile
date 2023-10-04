// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/compression/compression_error.hpp"

namespace tactile {

auto CompressionErrorDomain::get_message(const uint32 error_id) const noexcept
    -> StringView
{
  switch (Error {error_id}) {
    case Error::kNoData: return "No data to compress/decompress";
    case Error::kInternalError: return "An internal error occurred";
  }

  return "";
}

}  // namespace tactile