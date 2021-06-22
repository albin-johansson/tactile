#pragma once

#include <exception>  // exception

#include "aliases/czstring.hpp"

namespace tactile {

class TactileError final : public std::exception
{
 public:
  TactileError() noexcept = default;

  explicit TactileError(const czstring what) : mWhat{what ? what : "N/A"}
  {}

  [[nodiscard]] auto what() const -> czstring override
  {
    return mWhat;
  }

 private:
  czstring mWhat{"N/A"};
};

}  // namespace tactile
