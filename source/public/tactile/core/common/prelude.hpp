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

#include <cstddef>  // size_t, ptrdiff_t, uintptr_t
#include <cstdint>  // int{}_t, uint{}_t

#define TACTILE_VERSION_STRING "0.5.0 [WIP]"

// Build mode detection
#ifdef NDEBUG
  #define TACTILE_DEBUG 0
  #define TACTILE_RELEASE 1
#else
  #define TACTILE_DEBUG 1
  #define TACTILE_RELEASE 0
#endif  // NDEBUG

// Compiler detection
#ifdef _MSC_VER
  #define TACTILE_COMPILER_MSVC 1
#elif defined(__clang__)
  #define TACTILE_COMPILER_CLANG 1
#elif defined(__GNUC__)
  #define TACTILE_COMPILER_GCC 1
#else
  #warning "Unsupported compiler"
#endif  // _MSC_VER

#ifndef TACTILE_COMPILER_MSVC
  #define TACTILE_COMPILER_MSVC 0
#endif  // TACTILE_COMPILER_MSVC

#ifndef TACTILE_COMPILER_CLANG
  #define TACTILE_COMPILER_CLANG 0
#endif  // TACTILE_COMPILER_CLANG

#ifndef TACTILE_COMPILER_GCC
  #define TACTILE_COMPILER_GCC 0
#endif  // TACTILE_COMPILER_GCC

// Operating system detection
#ifdef _WIN32
  #define TACTILE_OS_WINDOWS 1
#elif defined(__APPLE__)
  #define TACTILE_OS_MACOS 1
#elif defined(__linux__)
  #define TACTILE_OS_LINUX 1
#else
  #warning "Unsupported operating system"
#endif  // _WIN32

#ifndef TACTILE_OS_WINDOWS
  #define TACTILE_OS_WINDOWS 0
#endif  // TACTILE_OS_WINDOWS

#ifndef TACTILE_OS_MACOS
  #define TACTILE_OS_MACOS 0
#endif  // TACTILE_OS_MACOS

#ifndef TACTILE_OS_LINUX
  #define TACTILE_OS_LINUX 0
#endif  // TACTILE_OS_LINUX

// 'noinline' function attribute
#if TACTILE_COMPILER_MSVC
  #define TACTILE_NOINLINE __declspec(noinline)
#elif TACTILE_COMPILER_CLANG
  #define TACTILE_NOINLINE [[clang::noinline]]
#elif TACTILE_COMPILER_GCC
  #define TACTILE_NOINLINE __attribute__((noinline))
#endif  // TACTILE_COMPILER_MSVC

// DLL symbol visibility macros
#if TACTILE_COMPILER_MSVC
  #define TACTILE_DLL_EXPORT __declspec(dllexport)
  #define TACTILE_DLL_IMPORT __declspec(dllimport)
#elif TACTILE_COMPILER_CLANG
  #define TACTILE_DLL_EXPORT __attribute__((__visibility__("default")))
  #define TACTILE_DLL_IMPORT
#elif TACTILE_COMPILER_GCC
  #define TACTILE_DLL_EXPORT __attribute__((dllexport))
  #define TACTILE_DLL_IMPORT __attribute__((dllimport))
#endif  // TACTILE_COMPILER_MSVC

#define TACTILE_DEFAULT_COPY(Name)   \
  Name(const Name& other) = default; \
  auto operator=(const Name& other) -> Name& = default

#define TACTILE_DELETE_COPY(Name)   \
  Name(const Name& other) = delete; \
  auto operator=(const Name& other) -> Name& = delete

#define TACTILE_DEFAULT_MOVE(Name)       \
  Name(Name&& other) noexcept = default; \
  auto operator=(Name&& other) noexcept -> Name& = default

#define TACTILE_DELETE_MOVE(Name)       \
  Name(Name&& other) noexcept = delete; \
  auto operator=(Name&& other) noexcept -> Name& = delete

#define TACTILE_FWD_DECLARE_CLASS(Name) class Name;
#define TACTILE_FWD_DECLARE_STRUCT(Name) struct Name;

namespace tactile {

using uchar = unsigned char;
using uint = unsigned int;
using ulonglong = unsigned long long int;

using usize = std::size_t;
using ssize = std::ptrdiff_t;
using uintptr = std::uintptr_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using float32 = float;
using float64 = double;

static_assert(sizeof(float32) == 4);
static_assert(sizeof(float64) == 8);

/** Strong type for object references. */
enum ObjectRef : int32 {
};

using LayerID = int32;
using ObjectID = int32;
using TileIndex = int32;  ///< For local tile identifiers.
using TileID = int32;     ///< For global tile identifiers.

inline constexpr TileID kEmptyTile = 0;

#if TACTILE_DEBUG
inline constexpr bool kIsDebugBuild = true;
inline constexpr bool kIsReleaseBuild = false;
#else
inline constexpr bool kIsDebugBuild = false;
inline constexpr bool kIsReleaseBuild = true;
#endif  // TACTILE_DEBUG

#if TACTILE_OS_WINDOWS
inline constexpr bool kOnWindows = true;
#else
inline constexpr bool kOnWindows = false;
#endif  // TACTILE_OS_WINDOWS

#if TACTILE_OS_MACOS
inline constexpr bool kOnMacos = true;
#else
inline constexpr bool kOnMacos = false;
#endif  // TACTILE_OS_MACOS

#if TACTILE_OS_LINUX
inline constexpr bool kOnLinux = true;
#else
inline constexpr bool kOnLinux = false;
#endif  // TACTILE_OS_LINUX

#ifdef TACTILE_BUILD_APP_BUNDLE
inline constexpr bool kIsAppBundle = true;
#else
inline constexpr bool kIsAppBundle = false;
#endif  // TACTILE_BUILD_APP_BUNDLE

}  // namespace tactile
