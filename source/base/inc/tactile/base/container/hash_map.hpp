// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <unordered_map>  // unordered_map

namespace tactile {

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

}  // namespace tactile
