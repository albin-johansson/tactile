// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <functional>
#include <map>

namespace tactile {

template <typename K, typename V>
using TreeMap = std::map<K, V, std::less<>>;

}  // namespace tactile
