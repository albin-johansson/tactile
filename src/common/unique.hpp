#pragma once

#include <memory>  // unique_ptr

namespace Tactile {

template <typename T>
using Unique = std::unique_ptr<T>;

}  // namespace Tactile
