// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <array>  // array

#include "tactile/core/basic/primitives.hpp"

namespace tactile {

template <typename T, usize N>
using Array = std::array<T, N>;

}  // namespace tactile
