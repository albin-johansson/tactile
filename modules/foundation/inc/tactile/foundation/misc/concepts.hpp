// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // same_as

#include "tactile/foundation/prelude.hpp"

namespace tactile {

template <typename T, typename... Args>
concept OneOf = (std::same_as<T, Args> || ...);

}  // namespace tactile
