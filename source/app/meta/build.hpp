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

#include <boost/config.hpp>
#include <boost/predef/compiler.h>
#include <boost/predef/os.h>

#define TACTILE_VERSION_STRING "0.5.0 [WIP]"

// Build mode
#ifdef NDEBUG
#define TACTILE_DEBUG 0
#define TACTILE_RELEASE 1
#else
#define TACTILE_DEBUG 1
#define TACTILE_RELEASE 0
#endif  // NDEBUG

// Compiler detection
#define TACTILE_COMPILER_MSVC BOOST_COMP_MSVC
#define TACTILE_COMPILER_CLANG BOOST_COMP_CLANG
#define TACTILE_COMPILER_GCC BOOST_COMP_GNUC

// OS detection
#define TACTILE_PLATFORM_WINDOWS BOOST_OS_WINDOWS
#define TACTILE_OS_WINDOWS BOOST_OS_WINDOWS
#define TACTILE_OS_LINUX BOOST_OS_LINUX
#define TACTILE_OS_MACOS BOOST_OS_MACOS

// Attributes
#define TACTILE_NOINLINE BOOST_NOINLINE

namespace tactile {

#if TACTILE_DEBUG
inline constexpr bool is_debug_build = true;
inline constexpr bool is_release_build = false;
#else
inline constexpr bool is_debug_build = false;
inline constexpr bool is_release_build = true;
#endif  // TACTILE_DEBUG

#if TACTILE_OS_WINDOWS
inline constexpr bool on_windows = true;
#else
inline constexpr bool on_windows = false;
#endif  // TACTILE_OS_WINDOWS

#if TACTILE_OS_MACOS
inline constexpr bool on_macos = true;
#else
inline constexpr bool on_macos = false;
#endif  // TACTILE_OS_MACOS

#if TACTILE_OS_LINUX
inline constexpr bool on_linux = true;
#else
inline constexpr bool on_linux = false;
#endif  // TACTILE_OS_LINUX

#ifdef TACTILE_BUILD_APP_BUNDLE
inline constexpr bool is_app_bundle = true;
#else
inline constexpr bool is_app_bundle = false;
#endif  // TACTILE_BUILD_APP_BUNDLE

}  // namespace tactile