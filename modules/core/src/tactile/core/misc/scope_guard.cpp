// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/misc/scope_guard.hpp"

#include <utility>  // move

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
  catch (...) {
  }
}

}  // namespace tactile