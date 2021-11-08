#pragma once

#include <tactile_def.hpp>

namespace Tactile {

auto ToolButton(CStr label,
                CStr tooltip,
                bool selected,
                bool enabled = true,
                float width = 0,
                float height = 0) -> bool;

}  // namespace Tactile
