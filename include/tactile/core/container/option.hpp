// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <optional>

namespace tactile {

template <typename T>
using Option = std::optional<T>;

inline constexpr std::nullopt_t kNone = std::nullopt;

using std::make_optional;

}  // namespace tactile
