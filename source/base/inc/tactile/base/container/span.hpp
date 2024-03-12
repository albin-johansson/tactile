// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <span>  // span, dynamic_extent

#include "tactile/base/int.hpp"

namespace tactile {

template <typename T, usize Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

}  // namespace tactile
