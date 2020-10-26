#pragma once

#include <exception>
#include <string>

#include "czstring.hpp"

namespace tactile {

class tactile_error final : public std::exception
{
 public:
  tactile_error() = default;

  /**
   * \brief Creates an exception with the supplied message.
   *
   * \param what the message of the exception. If the string is empty, "N/A"
   * is used.
   *
   * \since 0.1.0
   */
  explicit tactile_error(std::string what);

  [[nodiscard]] auto what() const noexcept -> czstring override;

 private:
  std::string m_what{"N/A"};
};

}  // namespace tactile
