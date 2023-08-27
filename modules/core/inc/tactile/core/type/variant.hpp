// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <variant>  // variant, monostate

namespace tactile {

template <typename... Types>
using Variant = std::variant<Types...>;

using Monostate = std::monostate;

}  // namespace tactile
