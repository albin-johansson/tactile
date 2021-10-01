#pragma once

#include <functional>  // reference_wrapper

namespace Tactile {

template <typename T>
using Ref = std::reference_wrapper<T>;

}  // namespace Tactile