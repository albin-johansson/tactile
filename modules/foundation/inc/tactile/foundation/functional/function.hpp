// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // function

namespace tactile {

template <typename T>
using Function = std::function<T>;

}  // namespace tactile