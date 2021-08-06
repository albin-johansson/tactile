#pragma once

#include "aliases/czstring.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

auto Checkbox(czstring label, NotNull<bool*> value, czstring tooltip = nullptr) -> bool;

}  // namespace Tactile