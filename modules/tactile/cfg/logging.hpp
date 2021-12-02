#pragma once

#include <tactile_def.hpp>

#include <centurion.hpp>

namespace Tactile {

void InitLogger();

void LogMessage(cen::log_priority priority, CStr msg);

}  // namespace Tactile
