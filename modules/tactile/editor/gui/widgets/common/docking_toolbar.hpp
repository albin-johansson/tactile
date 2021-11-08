#pragma once

#include <tactile_def.hpp>

namespace Tactile {

void BeginDockingToolbar(NotNull<CStr> name, int& axis);

void EndDockingToolbar();

}  // namespace Tactile
