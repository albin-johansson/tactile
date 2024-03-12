// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // less
#include <map>         // map

namespace tactile {

template <typename K, typename V>
using TreeMap = std::map<K, V, std::less<>>;

}  // namespace tactile
