module;

#include <chrono>

export module tactile.core:time;

export namespace tactile {

using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;

using SteadyClock = std::chrono::steady_clock;
using SystemClock = std::chrono::system_clock;

using std::chrono::duration_cast;

}  // namespace tactile
