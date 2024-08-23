// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>   // invocable
#include <exception>  // exception
#include <utility>    // move

#include "tactile/base/prelude.hpp"
#include "tactile/core/log/logger.hpp"

namespace tactile {

/**
 * RAII type used to invoke a callback at the end of a scope.
 *
 * \details
 * This class is useful for creating "inline" destructors without having to
 * define a new type. Such as in the following example.
 *
 * \code{cpp}
 * int i = 0;
 *
 * {
 *   const ScopeGuard guard {[&] { i = 10; }};
 *   i = 20;
 * }
 *
 * assert(i == 10);
 * \endcode
 */
template <std::invocable T>
class ScopeGuard final
{
 public:
  TACTILE_DELETE_COPY(ScopeGuard);
  TACTILE_DELETE_MOVE(ScopeGuard);

  /**
   * Creates a scope guard.
   *
   * \param callback The function object that will be called at the end of the
   *                 scope.
   */
  [[nodiscard]]
  explicit ScopeGuard(T callable) :
    mCallable {std::move(callable)}
  {}

  /**
   * Invokes the associated function object.
   */
  ~ScopeGuard() noexcept
  {
    try {
      mCallable();
    }
    catch (const std::exception& ex) {
      TACTILE_LOG_ERROR("Scope guard destructor threw exception: {}", ex.what());
    }
    catch (...) {
      TACTILE_LOG_ERROR("Scope guard destructor threw exception");
    }
  }

 private:
  T mCallable;
};

}  // namespace tactile
