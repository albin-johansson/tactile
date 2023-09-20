// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/profiler.hpp"

#include <exception>  // exception

#include <fmt/chrono.h>

#include "tactile/core/debug/log/logger.hpp"

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
    TACTILE_LOG_DEBUG("[PROFILER] Scope '{}' took {}", mDescription, duration);
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Scope profiler destructor threw unexpected exception: {}",
                      ex.what());
  }
}

}  // namespace tactile
