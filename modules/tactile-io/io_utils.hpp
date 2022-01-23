#pragma once

#include <string>  // string, erase, getline
#include <vector>  // vector

#include <tactile_def.hpp>

namespace tactile::IO {

[[nodiscard]] auto Split(CStr str, char sep) -> std::vector<std::string>;

}  // namespace tactile::IO