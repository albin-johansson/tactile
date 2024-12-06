// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile::common {

/// Forces initialization of the RNG engine for the current thread.
void random_init();

/// Returns a pseudo-random signed integer within the inclusive interval [min, max].
[[nodiscard]]
auto random_int(int min, int max) -> int;

/// Returns a pseudo-random unsigned integer within the inclusive interval [min, max].
[[nodiscard]]
auto random_uint(unsigned min, unsigned max) -> unsigned;

/// Returns a pseudo-random float within the inclusive interval [min, max].
[[nodiscard]]
auto random_double(double min, double max) -> double;

/// Returns a pseudo-random boolean.
[[nodiscard]]
auto random_bool() -> bool;

}  // namespace tactile::common
