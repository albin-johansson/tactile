#pragma once

#include <string>  // string, erase, getline
#include <vector>  // vector

#include <tactile_def.hpp>

namespace Tactile::IO {

[[nodiscard]] auto Split(CStr str, char sep) -> std::vector<std::string>;

}  // namespace Tactile::IO