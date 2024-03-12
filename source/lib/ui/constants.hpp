// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <limits>  // numeric_limits

#include "tactile/base/int.hpp"

namespace tactile {

inline constexpr auto kMinFloat = std::numeric_limits<float>::min();
inline constexpr auto kMaxFloat = std::numeric_limits<float>::max();

inline constexpr auto kMinInt = std::numeric_limits<int>::min();
inline constexpr auto kMaxInt = std::numeric_limits<int>::max();

inline constexpr auto kMaxUint = std::numeric_limits<uint>::max();

inline constexpr int32 kMinFontSize = 8;   ///< The smallest available font size.
inline constexpr int32 kMaxFontSize = 32;  ///< The largest available font size.
inline constexpr int32 kDefFontSize = 14;  ///< The default font size.

}  // namespace tactile
