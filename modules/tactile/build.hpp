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