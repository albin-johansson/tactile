#include "io_utils.hpp"

#include <sstream>  // stringstream

namespace Tactile::IO {

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

}  // namespace Tactile::IO