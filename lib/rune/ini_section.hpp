#ifndef RUNE_IO_INI_SECTION_HPP
#define RUNE_IO_INI_SECTION_HPP

#include <functional>   // less
#include <map>          // map
#include <ostream>      // ostream
#include <string>       // basic_string
#include <string_view>  // basic_string_view

#include "integers.hpp"
#include "rune_error.hpp"
#include "ini_value.hpp"

namespace rune {

/// \addtogroup io
/// \{

/// \name Ini
/// \{

template <typename Char>
struct ini_format final
{
  using value_type = Char;

  value_type section_start = '[';
  value_type section_end = ']';
  value_type assign = '=';
  value_type comment = ';';
};

template <typename Char>
class basic_ini_section final
{
 public:
  using char_type = Char;
  using value_type = basic_ini_value<char_type>;
  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;
  using format_type = ini_format<char_type>;
  using size_type = usize;

  void dump(std::ostream& stream, const format_type& format) const
  {
    for (const auto& [key, value] : m_entries)
    {
      stream << key << format.assign << value << '\n';
    }

    stream << '\n';
  }

  template <typename T>
  auto operator[](const T& element) -> value_type&
  {
    // TODO get_or_emplace with string_view_type parameter
    return m_entries[element];
  }

  // TODO erase

  [[nodiscard]] auto at(const string_view_type element) -> value_type&
  {
    if (const auto it = m_entries.find(element); it != m_entries.end())
    {
      return it->second;
    }
    else
    {
      throw rune_error{"basic_ini_section::at(): element does not exist!"};
    }
  }

  [[nodiscard]] auto at(const string_view_type element) const -> const value_type&
  {
    if (const auto it = m_entries.find(element); it != m_entries.end())
    {
      return it->second;
    }
    else
    {
      throw rune_error{"basic_ini_section::at(): element does not exist!"};
    }
  }

  [[nodiscard]] auto contains(const string_view_type element) const -> bool
  {
    return m_entries.find(element) != m_entries.end();
  }

  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_entries.size();
  }

  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_entries.empty();
  }

 private:
  std::map<string_type, value_type, std::less<>> m_entries;
};

/// \} End of ini

/// \} End of group IO

}  // namespace rune

#endif  // RUNE_IO_INI_SECTION_HPP
