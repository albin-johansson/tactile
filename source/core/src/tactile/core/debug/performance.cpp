// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/performance.hpp"

#include <exception>  // exception

#include "tactile/core/log/logger.hpp"

namespace tactile {

ScopeProfiler::ScopeProfiler(const char* description) noexcept
  : mDescription {description ? description : "?"},
    mStartInstant {SteadyClock::now()}
{}

ScopeProfiler::~ScopeProfiler() noexcept
{
  try {
    const auto end = SteadyClock::now();
    const auto duration = duration_cast<Microseconds>(end - mStartInstant);
    TACTILE_LOG_DEBUG("Scope '{}' took {}", mDescription, duration);
  }
  catch (const std::exception& e) {
    TACTILE_LOG_ERROR("Scope profiler destructor threw unexpected exception: {}", e.what());
  }
}

}  // namespace tactile
