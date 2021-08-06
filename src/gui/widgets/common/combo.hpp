#pragma once

#include "aliases/czstring.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

auto Combo(czstring label,
           czstring values,
           NotNull<int*> index,
           czstring tooltip = nullptr) -> bool;

}  // namespace Tactile