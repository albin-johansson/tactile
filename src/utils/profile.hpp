#pragma once

#include <centurion.hpp>

#include "build.hpp"

#if TACTILE_RELEASE
#define TACTILE_PROFILE_START
#else
#define TACTILE_PROFILE_START const auto tactile_profile_start = cen::counter::now();
#endif  // TACTILE_RELEASE

#if TACTILE_RELEASE
#define TACTILE_PROFILE_END(Msg)
#else
#define TACTILE_PROFILE_END(Msg)                                                    \
  const auto tactile_profile_end = cen::counter::now();                             \
  const auto tactile_profile_diff =                                                 \
      static_cast<double>(tactile_profile_end - tactile_profile_start);             \
  const auto tactile_profile_freq = static_cast<double>(cen::counter::frequency()); \
  CENTURION_LOG_DEBUG(Msg " in %f seconds", tactile_profile_diff / tactile_profile_freq);
#endif  // TACTILE_RELEASE