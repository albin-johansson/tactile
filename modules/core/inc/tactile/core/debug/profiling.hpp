// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

#define TACTILE_PROFILE_START \
  const auto tactile_profile_start = tactile::SystemClock::now();

#define TACTILE_PROFILE_END(Msg)                                                 \
  const auto tactile_profile_end = tactile::SystemClock::now();                  \
  const auto tactile_profile_diff = tactile_profile_end - tactile_profile_start; \
  const auto tactile_profile_ms =                                                \
      chrono::duration_cast<tactile::ms_t>(tactile_profile_diff);                \
  spdlog::debug(Msg " in {}", tactile_profile_ms);

#if TACTILE_DEBUG
  #define TACTILE_DEBUG_PROFILE_START TACTILE_PROFILE_START
  #define TACTILE_DEBUG_PROFILE_END(Msg) TACTILE_PROFILE_END(Msg)
#else
  #define TACTILE_DEBUG_PROFILE_START
  #define TACTILE_DEBUG_PROFILE_END(Msg)
#endif  // TACTILE_DEBUG
