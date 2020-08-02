#pragma once

#include <stdexcept>
#include <unordered_map>

#include "smart_pointers.hpp"

namespace tactile {

using czstring = const char*;

using BadArg [[deprecated]] = std::invalid_argument;

template <typename K, typename V>
using HashMap [[deprecated]] = std::unordered_map<K, V>;

}  // namespace tactile
