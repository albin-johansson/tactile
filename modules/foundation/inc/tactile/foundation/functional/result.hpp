// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_code

#include "tactile/foundation/functional/expected.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Alias for a value of expected type `T`, or an error code in unexpected cases.
 *
 * \see `Expected`
 */
template <typename T>
using Result = Expected<T, std::error_code>;

inline const Result<void> kSuccess {};

}  // namespace tactile
