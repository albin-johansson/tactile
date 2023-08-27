// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // reference_wrapper

namespace tactile {

template <typename T>
using Ref = std::reference_wrapper<T>;

}  // namespace tactile
