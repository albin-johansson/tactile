// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

// clang-format off

#define TACTILE_DELETE_COPY(Name) \
  Name(const Name&) = delete;     \
  auto operator=(const Name&) -> Name& = delete

#define TACTILE_DEFAULT_COPY(Name) \
  Name(const Name&) = default;     \
  auto operator=(const Name&) -> Name& = default

#define TACTILE_DELETE_MOVE(Name) \
  Name(Name&&) = delete;          \
  auto operator=(Name&&) -> Name& = delete

#define TACTILE_DEFAULT_MOVE(Name) \
  Name(Name&&) noexcept = default; \
  auto operator=(Name&&) noexcept -> Name& = default

#define TACTILE_INTERFACE(Name) \
 protected:                     \
  Name() noexcept = default;    \
  TACTILE_DEFAULT_COPY(Name);   \
  TACTILE_DEFAULT_MOVE(Name);   \
 public:                        \
  virtual ~Name() noexcept = default

// clang-format on
