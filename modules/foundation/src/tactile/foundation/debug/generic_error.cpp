// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/debug/generic_error.hpp"

namespace tactile {
namespace {

class GenericErrorCategory final : public std::error_category {
 public:
  [[nodiscard]]
  auto name() const noexcept -> const char* override
  {
    return "generic";
  }

  [[nodiscard]]
  auto message(const int value) const -> std::string override
  {
    switch (GenericError {value}) {
      case GenericError::kUnknown: return "unknown";
      case GenericError::kUnsupported: return "unsupported operation";
      case GenericError::kInvalidArgument: return "invalid argument";
      case GenericError::kInvalidState: return "invalid state";
      case GenericError::kInitFailure: return "initialization failure";
      case GenericError::kInvalidFile: return "invalid file";
      case GenericError::kNoSuchFile: return "no such file";
      case GenericError::kIOError: return "unknown I/O error";
    }

    return "?";
  }
};

}  // namespace

auto get_generic_error_category() noexcept -> const std::error_category&
{
  static GenericErrorCategory generic_error_category {};
  return generic_error_category;
}

}  // namespace tactile
