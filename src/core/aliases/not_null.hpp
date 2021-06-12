#pragma once

#include "is_pointer.hpp"

namespace tactile {

template <IsPointer T>
using not_null = T;

}  // namespace tactile
