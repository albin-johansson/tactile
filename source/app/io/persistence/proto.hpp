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

#include "meta/build.hpp"

#if TACTILE_COMPILER_MSVC

#pragma warning(push)
#pragma warning(disable : 4127)  // conditional expression is constant
#pragma warning(disable : 5054)  // operator '*' between enumerations of different types

#endif  // TACTILE_COMPILER_MSVC

#include <history.pb.h>
#include <session.pb.h>
#include <settings.pb.h>

#if TACTILE_COMPILER_MSVC

#pragma warning(pop)

#endif  // TACTILE_COMPILER_MSVC
