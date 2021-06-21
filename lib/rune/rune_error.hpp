#ifndef RUNE_CORE_RUNE_ERROR_HPP
#define RUNE_CORE_RUNE_ERROR_HPP

#include <exception>  // exception

namespace rune {

/// \addtogroup core
/// \{

class rune_error final : public std::exception
{
 public:
  explicit rune_error(const char* what) noexcept : m_what{what}
  {}

  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return m_what;
  }

 private:
  const char* m_what{"n/a"};
};

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_RUNE_ERROR_HPP
