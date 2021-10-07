#pragma once

#include <array>        // array
#include <format>       // format_to_n
#include <string_view>  // string_view

#include "common/cstr.hpp"
#include "common/ints.hpp"

namespace Tactile {

template <usize Capacity>
class FormattedString final
{
 public:
  template <typename... Args>
  explicit FormattedString(const std::string_view fmt, const Args&... args)
  {
    const auto res = std::format_to_n(m_buffer.data(), m_buffer.size() - 1, fmt, args...);
    *res.out = '\0';
    m_view = std::string_view{m_buffer.data(), res.out};
  }

  [[nodiscard]] auto data() const noexcept -> CStr
  {
    return m_buffer.data();
  }

  [[nodiscard]] auto view() const noexcept -> std::string_view
  {
    return m_view;
  }

 private:
  std::array<char, Capacity> m_buffer;  // NOLINT
  std::string_view m_view;
};

}  // namespace Tactile