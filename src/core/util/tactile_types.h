#pragma once
#include <stdexcept>
#include <unordered_map>

#include "smart_pointers.h"
#include "maybe.h"

namespace tactile {

using BadArg = std::invalid_argument;

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

}  // namespace tactile
