#pragma once

#include <exception>  // exception

#include "common/cstr.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class TactileError final : public std::exception
{
 public:
  TactileError() noexcept = default;

  explicit TactileError(const CStr what) : mWhat{what ? what : "N/A"}
  {}

  [[nodiscard]] auto what() const -> CStr override
  {
    return mWhat;
  }

 private:
  CStr mWhat{"N/A"};
};

/// \} End of group core

}  // namespace Tactile
