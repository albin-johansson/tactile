// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional, nullopt

namespace tactile {

template <typename T>
using Option = std::optional<T>;

using std::nullopt;

}  // namespace tactile
