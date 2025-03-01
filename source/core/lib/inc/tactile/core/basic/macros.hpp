// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#define TACTILE_VERSION_STRING "0.5.0"

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

// clang-format off

#define TACTILE_DECLARE_COPY_OPS(Name) \
  Name(const Name&);                   \
  auto operator=(const Name&) -> Name&

#define TACTILE_DEFINE_COPY_OPS(Name) \
  Name::Name(const Name&) = default;  \
  auto Name::operator=(const Name&) -> Name& = default

#define TACTILE_DECLARE_MOVE_OPS(Name) \
  Name(Name&&) noexcept;               \
  auto operator=(Name&&) noexcept -> Name&

#define TACTILE_DEFINE_MOVE_OPS(Name)    \
  Name::Name(Name&&) noexcept = default; \
  auto Name::operator=(Name&&) noexcept -> Name& = default

#define TACTILE_INLINE_COPY_OPS(Name, Op) \
  Name(const Name&) = Op;                 \
  auto operator=(const Name&) -> Name& = Op

#define TACTILE_INLINE_MOVE_OPS(Name, Op) \
  Name(Name&&) noexcept = Op;             \
  auto operator=(Name&&) noexcept -> Name& = Op

// clang-format on

#define TACTILE_DEFAULT_COPY(Name) TACTILE_INLINE_COPY_OPS(Name, default)
#define TACTILE_DEFAULT_MOVE(Name) TACTILE_INLINE_MOVE_OPS(Name, default)

#define TACTILE_DELETE_COPY(Name) TACTILE_INLINE_COPY_OPS(Name, delete)
#define TACTILE_DELETE_MOVE(Name) TACTILE_INLINE_MOVE_OPS(Name, delete)

#define TACTILE_INTERFACE_CLASS(Name) \
                                      \
 protected:                           \
  Name() noexcept = default;          \
  TACTILE_DEFAULT_COPY(Name);         \
  TACTILE_DEFAULT_MOVE(Name);         \
                                      \
 public:                              \
  virtual ~Name() noexcept = default

#define TACTILE_STATIC_CLASS(Name) \
                                   \
 public:                           \
  Name() = delete;                 \
  ~Name() = delete;                \
  TACTILE_DELETE_COPY(Name);       \
  TACTILE_DELETE_MOVE(Name)

#define TACTILE_PIMPL_CLASS(Name) \
  TACTILE_DELETE_COPY(Name);      \
  TACTILE_DECLARE_MOVE_OPS(Name); \
  ~Name() noexcept

#define TACTILE_DEFINE_PIMPL_CLASS(Name) \
  TACTILE_DEFINE_MOVE_OPS(Name);         \
  Name::~Name() noexcept = default
