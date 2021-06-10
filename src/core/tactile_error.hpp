#pragma once

#include <exception>  // exception

#include "czstring.hpp"

namespace tactile {

class TactileError final : public std::exception
{
 public:
  TactileError() noexcept = default;

  /**
   * \brief Creates an exception with the supplied message.
   *
   * \param what the message of the exception. If the string is empty, "N/A"
   * is used.
   *
   * \since 0.1.0
   */
  explicit TactileError(const czstring what) noexcept
      : mWhat{what ? what : "N/A"}
  {}

  [[nodiscard]] auto what() const noexcept -> czstring override
  {
    return mWhat;
  }

 private:
  czstring mWhat{"N/A"};
};

}  // namespace tactile
