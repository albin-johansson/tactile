// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

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
