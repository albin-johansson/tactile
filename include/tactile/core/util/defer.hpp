// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <utility>

#include "tactile/core/concepts.hpp"
#include "tactile/core/macros.hpp"

namespace tactile {

/// Simple RAII utility for writing "inline" destructors.
template <NothrowInvocable T>
class Defer final
{
 public:
  TACTILE_DELETE_COPY(Defer);
  TACTILE_DELETE_MOVE(Defer);

  explicit Defer(T&& callback) noexcept
    : m_callback {std::move(callback)}
  {}

  ~Defer() noexcept
  {
    m_callback();
  }

 private:
  T m_callback {};
};

}  // namespace tactile
