// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <unordered_map>

namespace tactile {

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

}  // namespace tactile
