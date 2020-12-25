#pragma once

#include <memory>  // unique_ptr, shared_ptr, weak_ptr

namespace tactile {

template <typename T>
using unique = std::unique_ptr<T>;

template <typename T>
using shared = std::shared_ptr<T>;

template <typename T>
using weak = std::weak_ptr<T>;

}  // namespace tactile
