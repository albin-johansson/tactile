// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/span.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/** A contiguous sequence of arbitrary bytes. */
using ByteSpan = Span<uint8>;

/** A contiguous stream of arbitrary bytes. */
using ByteStream = Vector<uint8>;

}  // namespace tactile
