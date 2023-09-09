// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <variant>  // variant, monostate, get, get_if, holds_alternative

namespace tactile {

template <typename... Types>
using Variant = std::variant<Types...>;

using Monostate = std::monostate;

using std::get;
using std::get_if;
using std::holds_alternative;

}  // namespace tactile
