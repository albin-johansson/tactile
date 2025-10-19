// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <chrono>

namespace tactile {

using SystemClock = std::chrono::system_clock;
using SteadyClock = std::chrono::steady_clock;

using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;

using std::chrono::duration_cast;

}  // namespace tactile
