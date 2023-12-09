// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/debug/error_code.hpp"
#include "tactile/foundation/functional/expected.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Alias for a value of expected type `T`, or an `ErrorCode` in unexpected cases.
 *
 * \see `Expected`
 */
template <typename T>
using Result = Expected<T, ErrorCode>;

inline const Result<void> kSuccess {};

}  // namespace tactile
