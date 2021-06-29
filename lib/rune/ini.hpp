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
#include <string>       // basic_string, getline
#include <string_view>  // basic_string_view
#include <utility>      // move
#include <vector>       // vector

#include "integers.hpp"
#include "maybe.hpp"
#include "rune_error.hpp"
#include "ini_section.hpp"
#include "ini_value.hpp"

namespace rune {

/// \addtogroup io
/// \{

/// \name Ini
/// \{

template <typename Char>
class basic_ini final
{
 public:
  using char_type = Char;
  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;
  using section_type = basic_ini_section<char_type>;
  using format_type = ini_format<char_type>;
  using size_type = usize;
  using iterator = typename std::map<string_type, section_type>::iterator;
  using const_iterator = typename std::map<string_type, section_type>::const_iterator;

  /**
   * \brief Creates an empty Ini file.
   *
   * \param format optional custom format.
   */
  explicit basic_ini(const format_type format = format_type{}) : m_format{format}
  {}

  /**
   * \brief Parses an `.ini` file based on an input stream.
   *
   * \param stream the input stream that will be read.
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
   * \brief Writes the contents of the instance to a stream.
   *
   * \param stream the output stream that will be used.
   */
  void write(std::ostream& stream) const
  {
    for (const auto& [name, values] : m_sections)
    {
      stream << m_format.section_start << name << m_format.section_end << '\n';
      values.dump(stream, m_format);
    }
  }

  /**
   * \brief Adds or replaces a section.
   *
   * \param section the name of the section that will be added or replaced.
   *
   * \return the new section.
   */
  auto emplace_or_replace(string_type section) -> section_type&
  {
    const auto [it, inserted] =
        m_sections.insert_or_assign(std::move(section), section_type{});
    return it->second;
  }

  /**
   * \brief Returns the section with the specified name if there is one, adding a new
   * section if it doesn't exist.
   *
   * \param section the name of the desired section.
   *
   * \return the existing section associated with the specified name or a new section if
   * no such section existed.
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

  void erase(const string_view_type section)
  {
    if (const auto it = m_sections.find(section); it != m_sections.end())
    {
      m_sections.erase(it);
    }
  }

  /**
   * \brief Returns the existing section associated with the specified name.
   *
   * \param section the name of the desired section.
   *
   * \return the section with the specified name.
   *
   * \throws rune_error if there is no section with the specified name.
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
   */
  [[nodiscard]] auto contains(const string_view_type section) const -> bool
  {
    return m_sections.find(section) != m_sections.end();
  }

  /**
   * \brief Returns the number of sections stored in the `.ini` file.
   *
   * \return the number of sections.
   */
  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_sections.size();
  }

  /**
   * \brief Indicates whether or not there are no sections.
   *
   * \return `true` if there are no sections; `false` otherwise.
   */
  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_sections.empty();
  }

  /**
   * \brief Indicates whether or not the parsed contents were valid.
   *
   * \return `true` if there are no parse errors associated with the instance; `false`
   * otherwise.
   */
  [[nodiscard]] explicit operator bool() const noexcept
  {
    return m_errors.empty();
  }

  [[nodiscard]] auto errors() const -> const std::vector<string_type>&
  {
    return m_errors;
  }

  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return m_sections.begin();
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_sections.begin();
  }

  [[nodiscard]] auto end() noexcept -> iterator
  {
    return m_sections.end();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_sections.end();
  }

 private:
  std::map<string_type, section_type, std::less<>> m_sections;
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

  [[nodiscard]] static auto is_unsigned(const string_type& str)
  {
    return !str.starts_with('-') && str.ends_with('u') &&
           std::all_of(str.begin(), str.end() - 1, [](const char_type ch) {
             return std::isdigit(ch, std::locale::classic());
           });
  }

  [[nodiscard]] static auto is_signed(const string_type& str)
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

/// Alias for the most likely `basic_ini` type.
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
 * \see `basic_ini::write()`
 */
template <typename Char>
auto operator<<(std::ostream& stream, const basic_ini<Char>& ini) -> std::ostream&
{
  ini.write(stream);
  return stream;
}

/**
 * \brief Parses an `.ini` file from an input stream.
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
 * \brief Parses an `.ini` file and returns its contents.
 *
 * \pre `path` must feature the `.ini` extension.
 *
 * \tparam Char the character type used.
 *
 * \param path the file path of the `.ini` file.
 *
 * \return the parsed contents of the `.ini` file.
 */
template <typename Char = char>
[[nodiscard]] auto read_ini(const std::filesystem::path& path) -> basic_ini<Char>
{
  assert(path.extension() == ".ini");
  std::ifstream stream{path};

  ini_file file;
  stream >> file;

  return file;
}

/**
 * \brief Saves an Ini file to the specified file path.
 *
 * \pre `path` must feature the `.ini` extension.
 *
 * \tparam Char the character type used.
 *
 * \param ini the Ini file that will be saved.
 * \param path the file path of the ini file.
 */
template <typename Char>
void write_ini(const basic_ini<Char>& ini, const std::filesystem::path& path)
{
  assert(path.extension() == ".ini");
  std::ofstream stream{path};
  stream << ini;
}

/// \} End of ini

/// \} End of group io

}  // namespace rune

#endif  // RUNE_IO_INI_HPP
