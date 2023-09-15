// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fmt/chrono.h>

#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

#define TACTILE_PROFILE_START \
  const auto tactile_profile_start = tactile::SteadyClock::now();

#define TACTILE_PROFILE_END(Msg)                                                 \
  const auto tactile_profile_end = tactile::SteadyClock::now();                  \
  const auto tactile_profile_diff = tactile_profile_end - tactile_profile_start; \
  const auto tactile_profile_ms =                                                \
      chrono::duration_cast<tactile::ms_t>(tactile_profile_diff);                \
  TACTILE_LOG_DEBUG(Msg " in {}", tactile_profile_ms);

#if TACTILE_DEBUG
  #define TACTILE_DEBUG_PROFILE_START TACTILE_PROFILE_START
  #define TACTILE_DEBUG_PROFILE_END(Msg) TACTILE_PROFILE_END(Msg)
#else
  #define TACTILE_DEBUG_PROFILE_START
  #define TACTILE_DEBUG_PROFILE_END(Msg)
#endif  // TACTILE_DEBUG
