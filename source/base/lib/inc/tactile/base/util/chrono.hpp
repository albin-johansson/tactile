// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <chrono>  // steady_clock, system_clock, duration_cast, milliseconds, microseconds

namespace tactile {

using SteadyClock = std::chrono::steady_clock;
using SteadyClockInstant = SteadyClock::time_point;
using SteadyClockDuration = SteadyClock::duration;

using SystemClock = std::chrono::system_clock;
using SystemClockInstant = SystemClock::time_point;
using SystemClockDuration = SystemClock::duration;

using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;

using std::chrono::duration_cast;

}  // namespace tactile
