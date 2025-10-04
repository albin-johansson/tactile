// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <variant>

namespace tactile {

using Monostate = std::monostate;

template <typename... Ts>
using Variant = std::variant<Ts...>;

using std::holds_alternative;
using std::get;
using std::get_if;

}  // namespace tactile
