// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <chrono>  // system_clock, nanoseconds, milliseconds, seconds

namespace tactile {

namespace chrono = std::chrono;

using SystemClock = chrono::system_clock;
using SystemClockInstant = SystemClock::time_point;

using ms_t = chrono::milliseconds;

}  // namespace tactile
