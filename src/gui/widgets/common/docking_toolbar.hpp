#pragma once

#include "aliases/cstr.hpp"
#include "aliases/not_null.hpp"

namespace Tactile {

void BeginDockingToolbar(NotNull<CStr> name, int& axis);

void EndDockingToolbar();

}  // namespace Tactile
