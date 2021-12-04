#pragma once

#include <string>  // string

namespace Tactile {

void UpdateLogDock();

void AddLogEntry(std::string msg);

void ClearLogEntries();

[[nodiscard]] auto IsLogDockFocused() noexcept -> bool;

}  // namespace Tactile
