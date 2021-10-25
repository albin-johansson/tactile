#pragma once

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

void BeginDockingToolbar(NotNull<CStr> name, int& axis);

void EndDockingToolbar();

}  // namespace Tactile
