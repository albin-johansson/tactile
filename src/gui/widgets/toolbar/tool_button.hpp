#pragma once

#include "common/cstr.hpp"

namespace Tactile {

auto ToolButton(CStr label, CStr tooltip, bool selected, bool enabled = true) -> bool;

}  // namespace Tactile
