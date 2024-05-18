// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/generic_error.hpp"

namespace tactile {
inline namespace generic_error {

class GenericErrorCategory final : public ErrorCategory
{
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
      case GenericError::kUnknown:      return "unknown";
      case GenericError::kUnsupported:  return "unsupported operation";
      case GenericError::kOutOfMemory:  return "out of memory";
      case GenericError::kInvalidParam: return "invalid parameter";
      case GenericError::kInvalidState: return "invalid state";
      case GenericError::kInvalidFile:  return "invalid file";
      case GenericError::kNoSuchFile:   return "no such file";
    }

    return "?";
  }
};

}  // namespace generic_error

auto get_generic_error_category() noexcept -> const ErrorCategory&
{
  static const GenericErrorCategory category {};
  return category;
}

}  // namespace tactile
