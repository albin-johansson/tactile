#pragma once

#include <tactile_def.hpp>

namespace tactile {

void BeginDockingToolbar(NotNull<CStr> name, int& axis);

void EndDockingToolbar();

}  // namespace tactile
