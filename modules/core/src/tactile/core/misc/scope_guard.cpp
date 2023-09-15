// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/misc/scope_guard.hpp"

#include <exception>  // exception
#include <utility>    // move

#include "tactile/core/debug/log/logger.hpp"

namespace tactile {

ScopeGuard::ScopeGuard(Callback callback) : mCallback {std::move(callback)}
{}

ScopeGuard::~ScopeGuard() noexcept
{
  try {
    if (mCallback) {
      mCallback();
    }
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Scope guard callback threw exception: {}", ex.what());
  }
  catch (...) {
    TACTILE_LOG_ERROR("Scope guard callback threw exception");
  }
}

}  // namespace tactile