// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Provides low-level status codes intended to be used across DLL boundaries.
 */
enum class StatusCode : uint8
{
  kOK,
  kUnsupported,
  kErrUnknown,
  kErrBadAlloc,
  kErrBadParam
};

}  // namespace tactile
