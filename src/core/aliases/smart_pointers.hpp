#pragma once

#include <memory>  // unique_ptr, shared_ptr, weak_ptr

namespace tactile {

template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

}  // namespace tactile
