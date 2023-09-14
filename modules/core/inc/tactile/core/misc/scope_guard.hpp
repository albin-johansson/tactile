// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/function.hpp"

namespace tactile {

/**
 * This class is used to invoke a callback at the end of a scope.
 *
 * \details The intended use-case is creating "inline" destructors without
 *          having to define a new type. See the below example.
 *
 *          \code{cpp}
 *          int i = 0;
 *
 *          {
 *            const ScopeGuard guard {[&] { i = 10; }};
 *            i = 20;
 *          }
 *
 *          assert(i == 10);
 *          \endcode
 */
class TACTILE_CORE_API ScopeGuard final {
 public:
  using Callback = Function<void()>;

  /** Creates a scope guard from the provided function object. */
  [[nodiscard]] explicit ScopeGuard(Callback callback);

  /** Invokes the associated function object, as long as it's valid. */
  ~ScopeGuard() noexcept;

  TACTILE_DELETE_COPY(ScopeGuard);
  TACTILE_DELETE_MOVE(ScopeGuard);

 private:
  Function<void()> mCallback;
};

}  // namespace tactile