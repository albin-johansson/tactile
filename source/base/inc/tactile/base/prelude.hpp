// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#define TACTILE_MAJOR_VERSION 0
#define TACTILE_MINOR_VERSION 5
#define TACTILE_PATCH_VERSION 0
#define TACTILE_VERSION_STRING "0.5.0 [WIP]"

// Build mode
#ifdef NDEBUG
  #define TACTILE_DEBUG 0
#else
  #define TACTILE_DEBUG 1
#endif

// Operating system
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

// Compiler
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
  #define TACTILE_NOINLINE __attribute__((noinline))
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
  #define TACTILE_DLL_EXPORT __attribute__((visibility("default")))
  #define TACTILE_DLL_IMPORT
#else
  #define TACTILE_DLL_EXPORT
  #define TACTILE_DLL_IMPORT
#endif

#define TACTILE_INIT_COPY(Name, Op) \
  Name(const Name&) = Op;           \
  auto operator=(const Name&)->Name& = Op

#define TACTILE_INIT_MOVE(Name, Op) \
  Name(Name&&) noexcept = Op;       \
  auto operator=(Name&&) noexcept -> Name& = Op

#define TACTILE_DEFAULT_COPY(Name) TACTILE_INIT_COPY(Name, default)
#define TACTILE_DEFAULT_MOVE(Name) TACTILE_INIT_MOVE(Name, default)

#define TACTILE_DELETE_COPY(Name) TACTILE_INIT_COPY(Name, delete)
#define TACTILE_DELETE_MOVE(Name) TACTILE_INIT_MOVE(Name, delete)

#define TACTILE_DECLARE_COPY(Name) \
  Name(const Name&);               \
  auto operator=(const Name&)->Name&

#define TACTILE_DECLARE_MOVE(Name) \
  Name(Name&&) noexcept;           \
  auto operator=(Name&&) noexcept -> Name&

#define TACTILE_DEFINE_COPY(Name)    \
  Name::Name(const Name&) = default; \
  auto Name::operator=(const Name&) -> Name& = default

#define TACTILE_DEFINE_MOVE(Name)        \
  Name::Name(Name&&) noexcept = default; \
  auto Name::operator=(Name&&) noexcept -> Name& = default

#define TACTILE_INTERFACE_CLASS(Name) \
                                      \
 protected:                           \
  Name() noexcept = default;          \
  TACTILE_DEFAULT_COPY(Name);         \
  TACTILE_DEFAULT_MOVE(Name);         \
                                      \
 public:                              \
  virtual Name() noexcept

#define TACTILE_STATIC_CLASS(Name) \
                                   \
 public:                           \
  Name() = delete;                 \
  ~Name() = delete;                \
  TACTILE_DELETE_COPY(Name);       \
  TACTILE_DELETE_MOVE(Name)

#define TACTILE_FWD_DECLARE_CLASS(Name) class Name;
#define TACTILE_FWD_DECLARE_STRUCT(Name) struct Name;

#define TACTILE_FWD_DECLARE_CLASS_NS(Ns, Name) \
  namespace Ns {                               \
  class Name;                                  \
  }

#define TACTILE_FWD_DECLARE_STRUCT_NS(Ns, Name) \
  namespace Ns {                                \
  struct Name;                                  \
  }

#define TACTILE_DECLARE_SPECIAL_MEMBERS(Class) \
  TACTILE_DECLARE_COPY(Class);                 \
  TACTILE_DECLARE_MOVE(Class);                 \
  ~Class()

namespace tactile {

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
