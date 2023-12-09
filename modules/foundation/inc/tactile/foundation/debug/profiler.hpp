// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/misc/chrono.hpp"
#include "tactile/foundation/prelude.hpp"

#if TACTILE_DEBUG
  #define TACTILE_DEBUG_PROFILE_SCOPE(Description)      \
    const tactile::ScopeProfiler tactile_scope_profiler \
    {                                                   \
      (Description)                                     \
    }
#else
  #define TACTILE_DEBUG_PROFILE_SCOPE(Description) \
    do {                                           \
    } while (false)
#endif  // TACTILE_DEBUG

namespace tactile {

/**
 * \brief RAII type used to measure the duration of scope.
 *
 * \details All required to use this class to profile a scope is to construct a profiler
 *          instance at the start of the scope (or wherever you want to start the
 *          measurement). The profiler destructor will subsequently log the duration
 *          of the scope.
 *
 *          \code{cpp}
 *          {
 *            const ScopeProfiler profiler {"foo::bar"};
 *
 *            // ...
 *
 *          } // Logs the duration of the scope
 *          \endcode
 */
class TACTILE_FOUNDATION_API ScopeProfiler final {
 public:
  TACTILE_DELETE_COPY(ScopeProfiler);
  TACTILE_DELETE_MOVE(ScopeProfiler);

  /**
   * \brief Starts the measurement.
   *
   * \param description an arbitrary description of the scope, included in the log output.
   */
  [[nodiscard]] explicit ScopeProfiler(const char* description) noexcept;

  /** \brief Ends the measurement and logs the result. */
  ~ScopeProfiler() noexcept;

 private:
  const char* mDescription;
  SteadyClockInstant mStartInstant;
};

}  // namespace tactile
