#pragma once

#ifdef NDEBUG
#define TACTILE_DEBUG 0
#else
#define TACTILE_DEBUG 1
#endif  // NDEBUG

#if TACTILE_DEBUG
#define TACTILE_RELEASE 0
#else
#define TACTILE_RELEASE 1
#endif  // TACTILE_DEBUG

#ifdef _MSC_VER
#define TACTILE_COMPILER_MSVC 1
#else
#define TACTILE_COMPILER_MSVC 0
#endif  // _MSC_VER

#ifdef __GNUC__
#define TACTILE_COMPILER_GCC 1
#else
#define TACTILE_COMPILER_GCC 0
#endif  // __GNUC__

#ifdef __clang__
#define TACTILE_COMPILER_CLANG 1
#else
#define TACTILE_COMPILER_CLANG 0
#endif  // __clang__

#ifdef WIN32
#define TACTILE_PLATFORM_WINDOWS 1
#else
#define TACTILE_PLATFORM_WINDOWS 0
#endif  // WIN32

#ifdef __linux__
#define TACTILE_PLATFORM_LINUX 1
#else
#define TACTILE_PLATFORM_LINUX 0
#endif  // __linux__

#ifdef __APPLE__
#define TACTILE_PLATFORM_OSX 1
#else
#define TACTILE_PLATFORM_OSX 0
#endif  // __APPLE__

namespace tactile {

#if TACTILE_DEBUG
inline constexpr bool is_debug_build = true;
inline constexpr bool is_release_build = false;
#else
inline constexpr bool is_debug_build = false;
inline constexpr bool is_release_build = true;
#endif  // TACTILE_DEBUG

#if TACTILE_PLATFORM_WINDOWS
inline constexpr bool on_windows = true;
#else
inline constexpr bool on_windows = false;
#endif  // TACTILE_PLATFORM_WINDOWS

#if TACTILE_PLATFORM_OSX
inline constexpr bool on_osx = true;
#else
inline constexpr bool on_osx = false;
#endif  // TACTILE_PLATFORM_OSX

}  // namespace tactile