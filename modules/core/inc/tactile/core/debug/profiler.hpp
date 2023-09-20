// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

#if TACTILE_DEBUG
  #define TACTILE_DEBUG_PROFILE_SCOPE(Description)      \
    const tactile::ScopeProfiler tactile_scope_profiler \
    {                                                   \
      (Description)                                     \
    }
#else
  #define TACTILE_DEBUG_PROFILE_SCOPE(Description)
#endif  // TACTILE_DEBUG

namespace tactile {

class TACTILE_CORE_API ScopeProfiler final {
 public:
  TACTILE_DELETE_COPY(ScopeProfiler);
  TACTILE_DELETE_MOVE(ScopeProfiler);

  [[nodiscard]] explicit ScopeProfiler(const char* description) noexcept;

  ~ScopeProfiler() noexcept;

 private:
  const char* mDescription;
  SteadyClockInstant mStartInstant;
};

}  // namespace tactile
