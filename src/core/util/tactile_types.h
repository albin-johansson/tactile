#pragma once
#include <stdexcept>
#include <unordered_map>

#include "maybe.h"
#include "smart_pointers.h"

namespace tactile {

using BadArg = std::invalid_argument;

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

}  // namespace tactile
