// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_code

#include "tactile/foundation/functional/expected.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

using ErrorCode = std::error_code;

/**
 * Alias for a value of expected type `T`, or an error code in unexpected cases.
 *
 * \see `Expected`
 */
template <typename T>
using Result = Expected<T, ErrorCode>;

inline const Result<void> kOK {};

}  // namespace tactile
