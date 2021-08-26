#ifndef INIT_INI_HPP
#define INIT_INI_HPP

#include <filesystem>   // path
#include <fstream>      // basic_ifstream, basic_ofstream
#include <functional>   // less
#include <istream>      // basic_istream
#include <map>          // map
#include <ostream>      // basic_ostream
#include <stdexcept>    // out_of_range
#include <string>       // basic_string, char_traits
#include <string_view>  // basic_string_view
#include <utility>      // move

#include "common.hpp"
#include "ini_parser.hpp"
#include "ini_section.hpp"
#include "ini_value.hpp"

namespace init {

/**
 * \class basic_ini
 *
 * \brief Represents an ini document.
 *
 * \tparam Char the character type that will be used.
 *
 * \see `ini`
 * \see `wini`
 * \see `read_ini()`
 * \see `write_ini()`
 *
 * \see `basic_ini_section`
 * \see `basic_ini_value`
 *
 * \since 0.1.0
 */
template <typename Char>
class basic_ini final
{
  friend class ini_parser<Char>;

 public:
  using char_type = Char;
  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;

  using parser_type = ini_parser<char_type>;
  using format_type = ini_format<char_type>;
  using section_type = basic_ini_section<char_type>;

  using storage_type = std::map<string_type, section_type, std::less<>>;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;

  using istream_type = std::basic_istream<char_type, std::char_traits<char_type>>;
  using ostream_type = std::basic_ostream<char_type, std::char_traits<char_type>>;
  using ifstream_type = std::basic_ifstream<char_type, std::char_traits<char_type>>;
  using ofstream_type = std::basic_ofstream<char_type, std::char_traits<char_type>>;
  using size_type = usize;

  explicit basic_ini(const format_type format = format_type{}) : m_format{format}
  {}

  basic_ini(const basic_ini&) = default;
  basic_ini(basic_ini&&) noexcept = default;

  basic_ini& operator=(const basic_ini&) = default;
  basic_ini& operator=(basic_ini&&) noexcept = default;

  /**
   * \brief Parses an ini document based on an input stream.
   *
   * \param stream the input stream that will be used.
   *
   * \since 0.1.0
   */
  void read(istream_type& stream)
  {
    parser_type parser;
    parser.parse_to(this, stream);
  }

  /**
   * \brief Writes the contents of the document to a stream.
   *
   * \param stream the output stream that will be used.
   *
   * \since 0.1.0
   */
  void dump(ostream_type& stream) const
  {
    for (const auto& [name, section] : m_sections)
    {
      stream << m_format.section_start << name << m_format.section_end << '\n';
      section.dump(stream, m_format);
    }
  }

  /**
   * \brief Creates or replaces a section.
   *
   * \param section the name of the section that will be created or replaced.
   *
   * \return the new section.
   *
   * \since 0.1.0
   */
  auto emplace_or_replace(string_type section) -> section_type&
  {
    const auto [it, inserted] =
        m_sections.insert_or_assign(std::move(section), section_type{});
    return it->second;
  }

  /**
   * \brief Retrieves a section from the file or default-constructs one if it doesn't
   * exist.
   *
   * \param section the name of the section that will be retrieved.
   *
   * \return the section that was either found or created.
   *
   * \since 0.1.0
   */
  auto get_or_emplace(const string_view_type section) -> section_type&
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      return it->second;
    }
    else
    {
      auto [iterator, inserted] = m_sections.try_emplace(string_type{section});
      return iterator->second;
    }
  }

  /// \copydoc get_or_emplace()
  auto operator[](const string_view_type section) -> section_type&
  {
    return get_or_emplace(section);
  }

  /**
   * \brief Returns the section associated with the specified name.
   *
   * \param section the name of the section to retrieve.
   *
   * \return the found section.
   *
   * \throws out_of_range if there is no section with the specified name.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto at(const string_view_type section) -> section_type&
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      return it->second;
    }
    else
    {
      throw std::out_of_range{"basic_ini::at(): section does not exist!"};
    }
  }

  /// \copydoc at()
  [[nodiscard]] auto at(const string_view_type section) const -> const section_type&
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      return it->second;
    }
    else
    {
      throw std::out_of_range{"basic_ini::at(): section does not exist!"};
    }
  }

  /**
   * \brief Removes a section from the document, if it exists.
   *
   * \param section the name of the section that will be removed.
   *
   * \return `true` if a section was removed; `false` otherwise.
   *
   * \since 0.1.0
   */
  auto erase(const string_view_type section) -> bool
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      m_sections.erase(it);
      return true;
    }
    else
    {
      return false;
    }
  }

  /**
   * \brief Indicates whether or not the specified section exists.
   *
   * \param section the name of the section to look for.
   *
   * \return `true` if the specified section exists; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto contains(const string_view_type section) const -> bool
  {
    return m_sections.find(section) != m_sections.end();
  }

  /**
   * \brief Returns the number of sections stored in the document.
   *
   * \return the number of sections.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_sections.size();
  }

  /**
   * \brief Indicates whether or not there are no sections.
   *
   * \return `true` if there are no sections; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_sections.empty();
  }

  /**
   * \brief Returns an iterator to the beginning.
   *
   * \return an iterator to the beginning.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return m_sections.begin();
  }

  /// \copydoc begin()
  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_sections.begin();
  }

  /**
   * \brief Returns an iterator to the end.
   *
   * \return an iterator to the end.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto end() noexcept -> iterator
  {
    return m_sections.end();
  }

  /// \copydoc end()
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_sections.end();
  }

 private:
  storage_type m_sections;
  format_type m_format;
};

/**
 * \typedef ini
 *
 * \brief Alias for ini documents based on `char`.
 *
 * \since 0.1.0
 */
using ini = basic_ini<char>;

/**
 * \typedef ini
 *
 * \brief Alias for ini documents based on `wchar_t`.
 *
 * \since 0.1.0
 */
using wini = basic_ini<wchar_t>;

/**
 * \brief Parses an ini file at the specified path.
 *
 * \tparam Char the character type that will be used.
 * \param path the file path of the ini file.
 *
 * \return the parsed ini document.
 *
 * \see `write_ini()`
 *
 * \since 0.1.0
 */
template <typename Char = char>
[[nodiscard]] auto read_ini(const std::filesystem::path& path) -> basic_ini<Char>
{
  using ifstream_type = typename basic_ini<Char>::ifstream_type;
  ifstream_type stream{path};

  basic_ini<Char> ini;
  ini.read(stream);

  return ini;
}

/**
 * \brief Saves an ini document to the specified file path.
 *
 * \tparam Char the character type used by the ini document.
 *
 * \param ini the ini document that will be saved.
 * \param path the destination file path of the ini document.
 *
 * \see `read_ini()`
 *
 * \since 0.1.0
 */
template <typename Char>
void write_ini(const basic_ini<Char>& ini, const std::filesystem::path& path)
{
  using ofstream_type = typename basic_ini<Char>::ofstream_type;

  ofstream_type stream{path};
  ini.dump(stream);
}

}  // namespace init

#endif  // INIT_INI_HPP
