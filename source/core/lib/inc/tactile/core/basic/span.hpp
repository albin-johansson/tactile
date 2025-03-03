// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <span>  // span

#include "tactile/core/basic/primitives.hpp"

namespace tactile {

template <typename T, usize Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

}  // namespace tactile
