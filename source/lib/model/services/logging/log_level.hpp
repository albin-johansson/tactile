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

#include "common/predef.hpp"
#include "common/primitives.hpp"

namespace tactile {

enum LogLevelBits : uint32 {
  LOG_LEVEL_TRACE = 1u << 0u,
  LOG_LEVEL_DEBUG = 1u << 1u,
  LOG_LEVEL_INFO = 1u << 2u,
  LOG_LEVEL_WARNING = 1u << 3u,
  LOG_LEVEL_ERROR = 1u << 4u,
  LOG_LEVEL_CRITICAL = 1u << 5u,
};

using LogLevelFlags = uint32;

inline constexpr LogLevelFlags kAllLogLevels = LOG_LEVEL_TRACE | LOG_LEVEL_DEBUG |
                                               LOG_LEVEL_INFO | LOG_LEVEL_WARNING |
                                               LOG_LEVEL_ERROR | LOG_LEVEL_CRITICAL;

#if TACTILE_DEBUG
inline constexpr LogLevelFlags kDefaultLogLevels = kAllLogLevels;
#else
inline constexpr LogLevelFlags kDefaultLogLevels = LOG_LEVEL_INFO |     //
                                                   LOG_LEVEL_WARNING |  //
                                                   LOG_LEVEL_ERROR |    //
                                                   LOG_LEVEL_CRITICAL;
#endif  // TACTILE_DEBUG

}  // namespace tactile
