#ifndef RUNE_IO_INI_HPP
#define RUNE_IO_INI_HPP

#include <algorithm>    // find_if, all_of
#include <cassert>      // assert
#include <filesystem>   // path
#include <fstream>      // ifstream, ofstream
#include <functional>   // less
#include <istream>      // istream
#include <locale>       // locale, isspace, isdigit
#include <map>          // map
#include <ostream>      // ostream
#include <sstream>      // istringstream
#include <string>       // basic_string, getline, stod, stoul, stoll
#include <string_view>  // basic_string_view
#include <utility>      // move
#include <vector>       // vector

#include "../aliases/integers.hpp"
#include "../aliases/maybe.hpp"
#include "../core/rune_error.hpp"
#include "ini_section.hpp"
#include "ini_value.hpp"

namespace rune {

/// \addtogroup io
/// \{

/// \name Ini
/// \{

/**
 * \brief Represents an ini file.
 *
 * \see `ini_file`
 * \see `basic_ini_section`
 * \see `basic_ini_value`
 * \see `write_ini()`
 * \see `read_ini()`
 * \see `operator<<(std::ostream&, const basic_ini&)`
 * \see `operator>>(std::istream&, basic_ini&)`
 *
 * \since 0.1.0
 */
template <typename Char>
class basic_ini final
{
 public:
  using char_type = Char;
  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;
  using section_type = basic_ini_section<char_type>;
  using format_type = ini_format<char_type>;
  using storage_type = std::map<string_type, section_type, std::less<>>;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using size_type = usize;

  /**
   * \brief Creates an empty ini file.
   *
   * \param format optional custom format.
   *
   * \since 0.1.0
   */
  explicit basic_ini(const format_type format = format_type{}) : m_format{format}
  {}

  /**
   * \brief Writes the contents of the file to a stream.
   *
   * \param stream the output stream that will be used.
   *
   * \since 0.1.0
   */
  void dump(std::ostream& stream) const
  {
    for (const auto& [name, section] : m_sections)
    {
      stream << m_format.section_start << name << m_format.section_end << '\n';
      section.dump(stream, m_format);
    }
  }

  /**
   * \brief Parses an ini file based on an input stream.
   *
   * \param stream the input stream that will be used.
   *
   * \since 0.1.0
   */
  void read(std::istream& stream)
  {
    string_type line;
    string_type section;

    line.reserve(32);
    section.reserve(32);

    while (std::getline(stream, line))
    {
      trim_left(line);
      trim_right(line);
      parse_line(line, section);
    }
  }

  /**
   * \brief Adds or replaces a section.
   *
   * \param section the name of the section that will be added or replaced.
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
   * already exist.
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
      assert(inserted);

      return iterator->second;
    }
  }

  /// \copydoc get_or_emplace()
  auto operator[](const string_view_type section) -> section_type&
  {
    return get_or_emplace(section);
  }

  /**
   * \brief Removes a section from the file, if it exists.
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
   * \brief Returns the section associated with the specified name.
   *
   * \param section the name of the section to retrieve.
   *
   * \return the found section.
   *
   * \throws rune_error if there is no section with the specified name.
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
      throw rune_error{"basic_ini::at(): section does not exist!"};
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
      throw rune_error{"basic_ini::at(): section does not exist!"};
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
   * \brief Returns the number of sections stored in the ini file.
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
   * \brief Indicates whether or not the parsed contents are valid.
   *
   * \return `true` if there are no errors associated with the file; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] explicit operator bool() const noexcept
  {
    return m_errors.empty();
  }

  /**
   * \brief Returns an iterator to the beginning of the instance.
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
   * \brief Returns an iterator to the end of the instance.
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
  std::vector<string_type> m_errors;
  format_type m_format;

  template <typename T>
  [[nodiscard]] static auto find_first_non_space(const T begin, const T end)
  {
    return std::find_if(begin, end, [](const char_type ch) {
      return !std::isspace(ch, std::locale::classic());
    });
  }

  // Remove leading whitespace
  void trim_left(string_type& line)
  {
    line.erase(line.begin(), find_first_non_space(line.begin(), line.end()));
  }

  // Remove any trailing whitespace
  void trim_right(string_type& line)
  {
    const auto it = find_first_non_space(line.rbegin(), line.rend());
    line.erase(it.base(), line.end());
  }

  [[nodiscard]] static auto is_float(const string_type& str) -> bool
  {
    // Require float values to feature dot
    if (str.find('.') == string_type::npos)
    {
      return false;
    }

    std::istringstream stream{str};

    double value{};
    stream >> value;

    return stream.eof() && !stream.fail();
  }

  [[nodiscard]] static auto is_unsigned(const string_type& str) -> bool
  {
    return !str.starts_with('-') && str.ends_with('u') &&
           std::all_of(str.begin(), str.end() - 1, [](const char_type ch) {
             return std::isdigit(ch, std::locale::classic());
           });
  }

  [[nodiscard]] static auto is_signed(const string_type& str) -> bool
  {
    if (str.starts_with('-'))
    {
      // Ignore leading minus
      return std::all_of(str.begin() + 1, str.end(), [](const char_type ch) {
        return std::isdigit(ch, std::locale::classic());
      });
    }
    else
    {
      return std::ranges::all_of(str, [](const char_type ch) {
        return std::isdigit(ch, std::locale::classic());
      });
    }
  }

  auto parse_variable(const string_type& line, const string_type& sectionName) -> bool
  {
    const auto assignment = std::ranges::find_if(line, [this](const char_type character) {
      return character == m_format.assign;
    });

    string_type variable{line.begin(), assignment};
    string_type value{assignment + 1, line.end()};

    trim_right(variable);
    trim_left(variable);

    auto& section = m_sections[sectionName];
    if (!section.contains(variable))
    {
      if (value == "true")
      {
        section[std::move(variable)] = true;
      }
      else if (value == "false")
      {
        section[std::move(variable)] = false;
      }
      else if (is_float(value))
      {
        section[std::move(variable)] = std::stod(value);
      }
      else if (is_unsigned(value))
      {
        section[std::move(variable)] = std::stoul(value);
      }
      else if (is_signed(value))
      {
        section[std::move(variable)] = std::stoll(value);
      }
      else
      {
        section[std::move(variable)] = std::move(value);
      }

      return true;
    }
    else
    {
      m_errors.push_back(line);
      return false;
    }
  }

  [[nodiscard]] auto parse_section_name(const string_type& line) -> maybe<string_type>
  {
    if (line.back() == m_format.section_end)
    {
      return line.substr(1, line.length() - 2);
    }
    else
    {
      m_errors.push_back(line);
      return nothing;
    }
  }

  void parse_line(const string_type& line, string_type& section)
  {
    if (line.empty())
    {
      return;
    }

    const auto& character = line.front();

    if (character == m_format.comment)
    {
      return;
    }

    if (character == m_format.section_start)
    {
      if (const auto name = parse_section_name(line))
      {
        section = *name;
      }
    }
    else if (parse_variable(line, section))
    {}
    else
    {
      m_errors.push_back(line);
    }
  }
};

/**
 * \brief Alias for the most common use case of `basic_ini`.
 *
 * \since 0.1.0
 */
using ini_file = basic_ini<char>;

/**
 * \brief Writes a `basic_ini` instance to an output stream.
 *
 * \tparam Char the character type used.
 *
 * \param stream the output stream that will be used.
 * \param ini the `basic_ini` instance that will be written to the stream.
 *
 * \return the used stream.
 *
 * \see `basic_ini::dump()`
 */
template <typename Char>
auto operator<<(std::ostream& stream, const basic_ini<Char>& ini) -> std::ostream&
{
  ini.dump(stream);
  return stream;
}

/**
 * \brief Parses an ini file from an input stream.
 *
 * \tparam Char the character type used.
 *
 * \param stream the input stream.
 * \param[out] ini the `basic_ini` instance that will be written to.
 *
 * \return the read input stream.
 *
 * \see `basic_ini::read()`
 * \see `read_ini()`
 */
template <typename Char>
auto operator>>(std::istream& stream, basic_ini<Char>& ini) -> std::istream&
{
  ini.read(stream);
  return stream;
}

/**
 * \brief Saves an Ini file to the specified file path.
 *
 * \tparam Char the character type used.
 *
 * \param ini the Ini file that will be saved.
 * \param path the file path of the ini file.
 *
 * \since 0.1.0
 */
template <typename Char>
void write_ini(const basic_ini<Char>& ini, const std::filesystem::path& path)
{
  std::ofstream stream{path};
  stream << ini;
}

/**
 * \brief Parses an ini file and returns its contents.
 *
 * \tparam Char the character type used.
 *
 * \param path the file path of the ini file.
 *
 * \return the parsed ini file; `nothing` if there were parsing errors.
 *
 * \since 0.1.0
 */
template <typename Char = char>
[[nodiscard]] auto read_ini(const std::filesystem::path& path) -> maybe<basic_ini<Char>>
{
  std::ifstream stream{path};

  ini_file file;
  stream >> file;

  if (file)
  {
    return file;
  }
  else
  {
    return nothing;
  }
}

/// \} End of ini

/// \} End of group io

}  // namespace rune

#endif  // RUNE_IO_INI_HPP
