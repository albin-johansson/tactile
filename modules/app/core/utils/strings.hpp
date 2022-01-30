#pragma once

#include <string>  // string
#include <vector>  // vector

namespace tactile {

[[nodiscard]] auto split(const char* str, char sep) -> std::vector<std::string>;

}  // namespace tactile
