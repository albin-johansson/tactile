// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.core:time;

export import std;

export namespace tactile {

using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;

using SteadyClock = std::chrono::steady_clock;
using SystemClock = std::chrono::system_clock;

using std::chrono::duration_cast;

}  // namespace tactile
