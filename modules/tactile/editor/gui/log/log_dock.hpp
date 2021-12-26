#pragma once

#include <string>  // string

namespace Tactile {

void UpdateLogDock();

[[nodiscard]] auto IsLogDockFocused() noexcept -> bool;

}  // namespace Tactile
