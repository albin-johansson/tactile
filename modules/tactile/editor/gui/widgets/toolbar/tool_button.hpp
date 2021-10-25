#pragma once

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

auto ToolButton(CStr label, CStr tooltip, bool selected, bool enabled = true) -> bool;

}  // namespace Tactile
