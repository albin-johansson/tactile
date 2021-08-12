#ifndef RUNE_CORE_RUNE_ERROR_HPP
#define RUNE_CORE_RUNE_ERROR_HPP

#include <exception>  // exception

#include "../aliases/str.hpp"

namespace rune {

/**
 * \brief The main exception thrown in the library.
 *
 * \ingroup core
 *
 * \since 0.1.0
 */
class rune_error final : public std::exception
{
 public:
  explicit rune_error(const str what) noexcept : m_what{what}
  {}

  [[nodiscard]] auto what() const noexcept -> str override
  {
    return m_what;
  }

 private:
  str m_what{"n/a"};
};

}  // namespace rune

#endif  // RUNE_CORE_RUNE_ERROR_HPP
