#ifndef INIT_INI_PARSER_HPP
#define INIT_INI_PARSER_HPP

#include <algorithm>  // all_of, find_if
#include <cassert>    // assert
#include <istream>    // basic_istream
#include <locale>     // locale, isspace, isdigit
#include <optional>   // optional
#include <sstream>    // istringstream
#include <string>     // basic_string, char_traits, getline, stod, stoul, stoll
#include <utility>    // move

#include "ini_section.hpp"
#include "ini_value.hpp"

namespace init {

template <typename Char>
class basic_ini;

template <typename Char>
class ini_parser final
{
 public:
  using char_type = Char;
  using string_type = std::basic_string<char_type>;
  using format_type = ini_format<char_type>;
  using ini_type = basic_ini<char_type>;
  using istream_type = std::basic_istream<char_type, std::char_traits<char_type>>;

  void parse_to(ini_type* ini, istream_type& stream)
  {
    assert(ini);
    m_ini = ini;

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

 private:
  ini_type* m_ini{};
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

    auto& section = m_ini->m_sections[sectionName];
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
      //      m_errors.push_back(line);
      return false;
    }
  }

  [[nodiscard]] auto parse_section_name(const string_type& line)
      -> std::optional<string_type>
  {
    if (line.back() == m_format.section_end)
    {
      return line.substr(1, line.length() - 2);
    }
    else
    {
      // m_errors.push_back(line);
      return std::nullopt;
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
      // m_errors.push_back(line);
    }
  }
};

}  // namespace init

#endif  // INIT_INI_PARSER_HPP
