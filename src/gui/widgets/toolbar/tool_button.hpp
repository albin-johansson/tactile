#pragma once

#include "aliases/czstring.hpp"

namespace Tactile {

auto ToolButton(czstring label, czstring tooltip, bool selected, bool enabled = true)
    -> bool;

}  // namespace Tactile
