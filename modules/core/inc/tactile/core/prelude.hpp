// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t, ptrdiff_t
#include <cstdint>  // uint{}_t, int{}_t, uintptr_t

#define TACTILE_VERSION_STRING "0.5.0 [WIP]"

// Build mode detection
#ifdef NDEBUG
  #define TACTILE_DEBUG 0
#else
  #define TACTILE_DEBUG 1
#endif

// Operating system detection
#if defined(_WIN32)
  #define TACTILE_OS_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
  #define TACTILE_OS_APPLE 1
#elif defined(__linux__)
  #define TACTILE_OS_LINUX 1
#else
  #warning "Unsupported operating system"
#endif

#ifndef TACTILE_OS_WINDOWS
  #define TACTILE_OS_WINDOWS 0
#endif

#ifndef TACTILE_OS_APPLE
  #define TACTILE_OS_APPLE 0
#endif

#ifndef TACTILE_OS_LINUX
  #define TACTILE_OS_LINUX 0
#endif

// Compiler detection
#if defined(_MSC_VER)
  #define TACTILE_COMPILER_MSVC 1
#elif defined(__clang__)
  #define TACTILE_COMPILER_CLANG 1
#elif defined(__GNUC__)
  #define TACTILE_COMPILER_GCC 1
#else
  #warning "Unsupported compiler"
#endif

#ifndef TACTILE_COMPILER_MSVC
  #define TACTILE_COMPILER_MSVC 0
#endif

#ifndef TACTILE_COMPILER_CLANG
  #define TACTILE_COMPILER_CLANG 0
#endif

#ifndef TACTILE_COMPILER_GCC
  #define TACTILE_COMPILER_GCC 0
#endif

#if TACTILE_COMPILER_MSVC
  #define TACTILE_NOINLINE __declspec(noinline)
#elif TACTILE_COMPILER_CLANG
  #define TACTILE_NOINLINE [[clang::noinline]]
#elif TACTILE_COMPILER_GCC
  #define TACTILE_NOINLINE __attribute__((noinline))
#else
  #define TACTILE_NOINLINE
#endif

#if TACTILE_COMPILER_MSVC
  #define TACTILE_DLL_EXPORT __declspec(dllexport)
  #define TACTILE_DLL_IMPORT __declspec(dllimport)
#elif TACTILE_COMPILER_CLANG
  #define TACTILE_DLL_EXPORT __attribute__((__visibility__("default")))
  #define TACTILE_DLL_IMPORT
#elif TACTILE_COMPILER_GCC
  #define TACTILE_DLL_EXPORT __attribute__((dllexport))
  #define TACTILE_DLL_IMPORT __attribute__((dllimport))
#else
  #define TACTILE_DLL_EXPORT
  #define TACTILE_DLL_IMPORT
#endif

#define TACTILE_INIT_COPY(Name, Op) \
  Name(const Name& other) = Op;     \
  auto operator=(const Name& other) -> Name& = Op

#define TACTILE_INIT_MOVE(Name, Op) \
  Name(Name&& other) noexcept = Op; \
  auto operator=(Name&& other) noexcept -> Name& = Op

#define TACTILE_DEFAULT_COPY(Name) TACTILE_INIT_COPY(Name, default)
#define TACTILE_DEFAULT_MOVE(Name) TACTILE_INIT_MOVE(Name, default)

#define TACTILE_DELETE_COPY(Name) TACTILE_INIT_COPY(Name, delete)
#define TACTILE_DELETE_MOVE(Name) TACTILE_INIT_MOVE(Name, delete)

#define TACTILE_INTERFACE_CLASS(Name) \
  Name() = default;                   \
  TACTILE_DEFAULT_COPY(Name);         \
  TACTILE_DEFAULT_MOVE(Name);         \
  virtual ~Name() noexcept = default

namespace tactile {

using usize = std::size_t;
using ssize = std::ptrdiff_t;

using uchar = unsigned char;
using uint = unsigned int;
using ulonglong = unsigned long long int;
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

using LayerID = int32;
using ObjectID = int32;
using TileIndex = int32;  ///< For local tile identifiers.
using TileID = int32;     ///< For global tile identifiers.

inline constexpr TileID kEmptyTile = 0;

namespace int_literals {

[[nodiscard]] constexpr auto operator"" _uz(const ulonglong value) noexcept -> usize
{
  return static_cast<usize>(value);
}

}  // namespace int_literals
}  // namespace tactile
