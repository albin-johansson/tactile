#pragma once

#include "tactile.hpp"

namespace tactile {

auto ToolButton(c_str label,
                c_str tooltip,
                bool selected,
                bool enabled = true,
                float width = 0,
                float height = 0) -> bool;

}  // namespace tactile
