#pragma once

#include "common/cstr.hpp"
#include "common/not_null.hpp"

namespace Tactile {

void BeginDockingToolbar(NotNull<CStr> name, int& axis);

void EndDockingToolbar();

}  // namespace Tactile
