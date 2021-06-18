#pragma once

#include "is_pointer.hpp"

namespace tactile {

template <IsPointer T>
using NotNull = T;

}  // namespace tactile
