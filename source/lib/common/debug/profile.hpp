/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <chrono>  // duration_cast

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include "common/type/chrono.hpp"
#include "tactile/base/prelude.hpp"

#define TACTILE_PROFILE_START const auto tactile_profile_start = tactile::Clock::now();

#define TACTILE_PROFILE_END(Msg)                                                 \
  const auto tactile_profile_end = tactile::Clock::now();                        \
  const auto tactile_profile_diff = tactile_profile_end - tactile_profile_start; \
  const auto tactile_profile_ms =                                                \
      std::chrono::duration_cast<tactile::ms_t>(tactile_profile_diff);           \
  spdlog::debug(Msg " in {}", tactile_profile_ms);

#if TACTILE_DEBUG
  #define TACTILE_DEBUG_PROFILE_START TACTILE_PROFILE_START
  #define TACTILE_DEBUG_PROFILE_END(Msg) TACTILE_PROFILE_END(Msg)
#else
  #define TACTILE_DEBUG_PROFILE_START
  #define TACTILE_DEBUG_PROFILE_END(Msg)
#endif  // TACTILE_DEBUG
