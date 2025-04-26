// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/special_member_functions.hpp"

#define TACTILE_INTERFACE_CLASS(Name) \
                                      \
 protected:                           \
  Name() noexcept = default;          \
  TACTILE_DEFAULT_COPY(Name);         \
  TACTILE_DEFAULT_MOVE(Name);         \
                                      \
 public:                              \
  virtual ~Name() noexcept = default
