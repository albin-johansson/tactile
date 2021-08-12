#ifndef RUNE_IO_INI_SECTION_HPP
#define RUNE_IO_INI_SECTION_HPP

#include <functional>   // less
#include <map>          // map
#include <ostream>      // ostream
#include <string>       // basic_string
#include <string_view>  // basic_string_view

#include "../aliases/integers.hpp"
#include "../core/rune_error.hpp"
#include "ini_value.hpp"

namespace rune {

/// \addtogroup io
/// \{

/// \name Ini
/// \{

/**
 * \brief Represents the syntax used by an ini file.
 *
 * \tparam Char the character type that is used.
 *
 * \since 0.1.0
 */
template <typename Char>
struct ini_format final
{
  using value_type = Char;

  value_type section_start = '[';  ///< Token introducing a section name.
  value_type section_end = ']';    ///< Token that ends a section name.
  value_type assign = '=';         ///< Assignment operator token.
  value_type comment = ';';        ///< Line comment token.
};

/**
 * \brief Represents a section in an ini file.
 *
 * \tparam Char the character type that is used.
 *
 * \see `ini_section`
 * \see `basic_ini`
 * \see `basic_ini_value`
 *
 * \since 0.1.0
 */
template <typename Char>
class basic_ini_section final
{
 public:
  using char_type = Char;
  using elem_type = basic_ini_value<char_type>;
  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;
  using format_type = ini_format<char_type>;
  using storage_type = std::map<string_type, elem_type, std::less<>>;
  using value_type = typename storage_type::value_type;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using size_type = usize;

  /**
   * \brief Outputs the contents of the section to an output stream.
   *
   * \param stream the output stream that will be used.
   * \param format the syntax that will be used.
   *
   * \since 0.1.0
   */
  void dump(std::ostream& stream, const format_type& format) const
  {
    for (const auto& [key, value] : m_entries)
    {
      stream << key << format.assign << value << '\n';
    }

    stream << '\n';
  }

  /**
   * \brief Removes an element from the section, if it exists.
   *
   * \param element the name of the element that will be removed.
   *
   * \return `true` if an element was removed; `false` otherwise.
   *
   * \since 0.1.0
   */
  auto erase(const string_view_type element) -> bool
  {
    if (const auto it = m_entries.find(element); it != m_entries.end())
    {
      m_entries.erase(it);
      return true;
    }
    else
    {
      return false;
    }
  }

  /**
   * \brief Retrieves an element from the section or default-constructs one if it doesn't
   * already exist.
   *
   * \param element the name of the element that will be retrieved.
   *
   * \return the value that was either found or created.
   *
   * \since 0.1.0
   */
  auto get_or_emplace(const string_view_type element) -> elem_type&
  {
    if (const auto it = m_entries.find(element); it != m_entries.end())
    {
      return it->second;
    }
    else
    {
      return m_entries[string_type{element}];
    }
  }

  /// \copydoc get_or_emplace()
  auto operator[](const string_view_type element) -> elem_type&
  {
    return get_or_emplace(element);
  }

  /**
   * \brief Returns the value associated with the specified name.
   *
   * \param element the name of the value to obtain.
   *
   * \return the ini value associated with the specified name.
   *
   * \throws rune_error if the section doesn't contain the specified value.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto at(const string_view_type element) -> elem_type&
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

  /// \copydoc at()
  [[nodiscard]] auto at(const string_view_type element) const -> const elem_type&
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

  /**
   * \brief Indicates whether or not the section contains a value with the specified name.
   *
   * \param element the name of the element to look for.
   *
   * \return `true` if the section contains a value with the specified name; `false`
   * otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto contains(const string_view_type element) const -> bool
  {
    return m_entries.find(element) != m_entries.end();
  }

  /**
   * \brief Returns the amount of values present in the section.
   *
   * \return the amount of values in the section.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_entries.size();
  }

  /**
   * \brief Indicates whether or not the section is empty.
   *
   * \return `true` if the section is empty; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_entries.empty();
  }

  /**
   * \brief Returns an iterator to the beginning of the section.
   *
   * \return an iterator to the beginning.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return m_entries.begin();
  }

  /**
   * \brief Returns a const iterator to the beginning of the section.
   *
   * \return a const iterator to the beginning.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_entries.begin();
  }

  /**
   * \brief Returns an iterator to the end of the section.
   *
   * \return an iterator to the end.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto end() noexcept -> iterator
  {
    return m_entries.end();
  }

  /**
   * \brief Returns a const iterator to the end of the section.
   *
   * \return a const iterator to the end.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_entries.end();
  }

 private:
  storage_type m_entries;
};

/**
 * \brief Alias for the most common use case of `basic_ini_section`.
 *
 * \since 0.1.0
 */
using ini_section = basic_ini_section<char>;

/// \} End of ini

/// \} End of group IO

}  // namespace rune

#endif  // RUNE_IO_INI_SECTION_HPP
