#include "string_utils.hpp"

#include <cassert>  // assert
#include <sstream>  // stringstream
#include <string>   // erase, getline

namespace Tactile {

auto Split(const CStr str, const char sep) -> std::vector<std::string>
{
  std::stringstream stream{str};
  std::vector<std::string> tokens;

  std::string token;
  while (std::getline(stream, token, sep)) {
    std::erase(token, '\n');
    tokens.push_back(std::move(token));
    token.clear();
  }

  return tokens;
}

}  // namespace Tactile