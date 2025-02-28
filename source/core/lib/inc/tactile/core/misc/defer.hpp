// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // function
#include <utility>     // move

#include "tactile/core/basic/macros.hpp"

namespace tactile {

/// Utility for invoking a function object at scope exit.
class Defer final
{
 public:
  using function_type = std::function<void()>;

  [[nodiscard]]
  explicit Defer(function_type&& callable)
    : m_callable {std::move(callable)}
  {}

  ~Defer() noexcept
  {
    m_callable();
  }

  TACTILE_DELETE_COPY(Defer);
  TACTILE_DELETE_MOVE(Defer);

 private:
  function_type m_callable;
};

}  // namespace tactile
