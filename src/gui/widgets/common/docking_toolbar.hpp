#pragma once

#include "aliases/czstring.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

void BeginDockingToolbar(NotNull<czstring> name, int& axis);

void EndDockingToolbar();

}  // namespace Tactile
