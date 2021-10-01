#pragma once

#include <unordered_map>  // unordered_map

namespace Tactile {

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

}  // namespace Tactile
