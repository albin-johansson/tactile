// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/compress/compression_error.hpp"

namespace tactile {
namespace {

class CompressionErrorCategory final : public std::error_category
{
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
      case CompressionError::kBadInit:            return "initialization failed";
      case CompressionError::kBadCleanup:         return "cleanup failed";
      case CompressionError::kBadState:           return "invalid state";
      case CompressionError::kCouldNotCompress:   return "compression failed";
      case CompressionError::kCouldNotDecompress: return "decompression failed";
    }

    return "?";
  }
};

}  // namespace

auto get_compression_error_category() noexcept -> const std::error_category&
{
  static CompressionErrorCategory category {};
  return category;
}

}  // namespace tactile
