// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <span>  // span, dynamic_extent

#include "tactile/foundation/prelude.hpp"

namespace tactile {

template <typename T, usize Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

}  // namespace tactile
