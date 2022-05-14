/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <centurion/system.hpp>
#include <spdlog/spdlog.h>

#include "meta/build.hpp"

#if TACTILE_RELEASE
#define TACTILE_PROFILE_START
#else
#define TACTILE_PROFILE_START const auto tactile_profile_start = cen::now();
#endif  // TACTILE_RELEASE

#if TACTILE_RELEASE
#define TACTILE_PROFILE_END(Msg)
#else
#define TACTILE_PROFILE_END(Msg)                                           \
  const auto tactile_profile_end = cen::now();                             \
  const auto tactile_profile_diff =                                        \
      static_cast<double>(tactile_profile_end - tactile_profile_start);    \
  const auto tactile_profile_freq = static_cast<double>(cen::frequency()); \
  spdlog::debug(Msg " in {0:.6f} seconds", tactile_profile_diff / tactile_profile_freq);
#endif  // TACTILE_RELEASE