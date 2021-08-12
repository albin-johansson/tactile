#ifndef RUNE_IO_CSV_HPP
#define RUNE_IO_CSV_HPP

#include <sstream>  // istringstream
#include <string>   // string, getline
#include <utility>  // move
#include <vector>   // vector

namespace rune {

/**
 * \brief Parses a string of comma-separated values (CSV).
 *
 * \ingroup io
 *
 * \param csv the string that holds the comma separated values.
 *
 * \return the parsed tokens.
 *
 * \since 0.1.0
 */
[[nodiscard]] inline auto parse_csv(const std::string& csv) -> std::vector<std::string>
{
  std::vector<std::string> tokens;

  std::istringstream stream{csv};
  std::string token;

  while (std::getline(stream, token, ','))
  {
    tokens.push_back(std::move(token));
    token.clear();
  }

  return tokens;
}

}  // namespace rune

#endif  // RUNE_IO_CSV_HPP
