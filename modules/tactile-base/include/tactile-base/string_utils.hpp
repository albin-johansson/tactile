#pragma once

#include <sstream>  // stringstream
#include <string>   // string, erase, getline
#include <vector>   // vector

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

[[nodiscard]] inline auto Split(CStr str, char sep) -> std::vector<std::string>
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