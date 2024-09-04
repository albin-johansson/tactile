// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // function
#include <utility>     // move, exchange

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Utility for invoking a function object at scope exit.
 */
class ScopeExit final
{
 public:
  using function_type = std::function<void()>;

  TACTILE_DELETE_COPY(ScopeExit);

  [[nodiscard]]
  ScopeExit() = default;

  [[nodiscard]]
  explicit ScopeExit(function_type&& callable)
      : m_callable {std::move(callable)},
        m_primed {true}
  {}

  [[nodiscard]]
  ScopeExit(ScopeExit&& other) noexcept
      : m_callable {std::move(other.m_callable)},
        m_primed {std::exchange(other.m_primed, false)}
  {}

  ~ScopeExit() noexcept
  {
    _try_call();
  }

  auto operator=(ScopeExit&& other) noexcept -> ScopeExit&
  {
    if (this != &other) {
      _try_call();

      m_callable = std::move(other.m_callable);
      m_primed = std::exchange(other.m_primed, false);
    }

    return *this;
  }

 private:
  function_type m_callable {};
  bool m_primed {false};

  void _try_call() noexcept
  {
    if (std::exchange(m_primed, false)) {
      m_callable();
    }
  }
};

}  // namespace tactile
