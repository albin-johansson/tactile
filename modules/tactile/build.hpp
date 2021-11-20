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