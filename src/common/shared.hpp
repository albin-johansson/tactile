#pragma once

#include <memory>  // shared_ptr

namespace Tactile {

template <typename T>
using Shared = std::shared_ptr<T>;

}  // namespace Tactile
