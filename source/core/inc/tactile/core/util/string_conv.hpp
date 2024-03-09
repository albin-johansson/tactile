// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/**
 * Attempts to convert a string to an integer.
 *
 * \param str  The source string.
 * \param base The numerical base to use.
 *
 * \return
 *    An integer if successful; an error code otherwise.
 */
[[nodiscard]]
auto parse_int(StringView str, int base = 10) -> Result<int64>;

/** \copydoc parse_int */
[[nodiscard]]
auto parse_uint(StringView str, int base = 10) -> Result<uint64>;

/**
 * Attempts to convert a string to a float.
 *
 * \param str  The source string.
 *
 * \return
 *    A float if successful; an error code otherwise.
 */
[[nodiscard]]
auto parse_float(StringView str) -> Result<float>;

}  // namespace tactile
