#pragma once

#include <string>  // string
#include <vector>  // vector

#include "common/cstr.hpp"
#include "common/not_null.hpp"

namespace Tactile {

[[nodiscard]] auto Split(CStr str, char sep) -> std::vector<std::string>;

}  // namespace Tactile