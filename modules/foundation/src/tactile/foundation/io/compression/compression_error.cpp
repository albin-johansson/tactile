// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/compression/compression_error.hpp"

namespace tactile {
namespace {

class CompressionErrorCategory final : public std::error_category {
 public:
  [[nodiscard]]
  auto name() const noexcept -> const char* override
  {
    return "compression";
  }

  [[nodiscard]]
  auto message(const int value) const -> std::string override
  {
    switch (CompressionError {value}) {
      case CompressionError::kNoData: return "no data to compress or decompress";
      case CompressionError::kInternalError: return "an internal error occurred";
      case CompressionError::kInvalidMode: return "detected an invalid compression mode";
    }

    return "?";
  }
};

}  // namespace

auto get_compression_error_category() noexcept -> const std::error_category&
{
  static CompressionErrorCategory compression_error_category {};
  return compression_error_category;
}

}  // namespace tactile