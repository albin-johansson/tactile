// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional, nullopt_t, nullopt

namespace tactile {

template <typename T>
using Maybe = std::optional<T>;

inline constexpr std::nullopt_t kNone = std::nullopt;

}  // namespace tactile
