#ifndef INIT_INI_SECTION_HPP
#define INIT_INI_SECTION_HPP

#include <functional>   // less
#include <map>          // map
#include <ostream>      // basic_ostream
#include <stdexcept>    // out_of_range
#include <string>       // basic_string, char_traits
#include <string_view>  // basic_string_view

#include "common.hpp"
#include "ini_value.hpp"

namespace init {

/**
 * \struct ini_format
 *
 * \brief Represents the syntax used by an ini file.
 *
 * \tparam Char the character type that is used.
 *
 * \since 0.1.0
 */
template <typename Char>
struct ini_format final
{
  using char_type = Char;

  char_type section_start = '[';  ///< Token introducing a section name.
  char_type section_end = ']';    ///< Token that ends a section name.
  char_type assign = '=';         ///< Assignment operator token.
  char_type comment = ';';        ///< Line comment token.
};

/**
 * \class basic_ini_section
 *
 * \brief Represents a section in an ini file.
 *
 * \tparam Char the character type that is used.
 *
 * \see `ini_section`
 * \see `wini_section`
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
  using format_type = ini_format<char_type>;

  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;
  using ostream_type = std::basic_ostream<char_type, std::char_traits<char_type>>;

  using storage_type = std::map<string_type, elem_type, std::less<>>;
  using value_type = typename storage_type::value_type;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using size_type = usize;

  basic_ini_section() = default;

  basic_ini_section(const basic_ini_section&) = default;
  basic_ini_section(basic_ini_section&&) noexcept = default;

  basic_ini_section& operator=(const basic_ini_section&) = default;
  basic_ini_section& operator=(basic_ini_section&&) noexcept = default;

  /**
   * \brief Writes the contents of the section to an output stream.
   *
   * \param stream the output stream that will be used.
   * \param format the syntax that will be used.
   *
   * \since 0.1.0
   */
  void dump(ostream_type& stream, const format_type& format) const
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
   * exist.
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
   * \throws out_of_range if the section doesn't contain the specified value.
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
      throw std::out_of_range{"basic_ini_section::at(): element does not exist!"};
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
      throw std::out_of_range{"basic_ini_section::at(): element does not exist!"};
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

  /// \copydoc begin()
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

  /// \copydoc end()
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_entries.end();
  }

 private:
  storage_type m_entries;
};

/**
 * \typedef ini_section
 *
 * \brief Alias for ini sections based on `char`.
 *
 * \since 0.1.0
 */
using ini_section = basic_ini_section<char>;

/**
 * \typedef ini_section
 *
 * \brief Alias for ini sections based on `wchar_t`.
 *
 * \since 0.1.0
 */
using wini_section = basic_ini_section<wchar_t>;

}  // namespace init

#endif  // INIT_INI_SECTION_HPP
