#pragma once

#include <memory>  // shared_ptr

namespace tactile {

template <typename T>
using Shared = std::shared_ptr<T>;

}  // namespace tactile
