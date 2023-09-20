// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr, shared_ptr, weak_ptr, default_delete, make_unique, make_shared

namespace tactile {

template <typename T, typename Deleter = std::default_delete<T>>
using Unique = std::unique_ptr<T, Deleter>;

template <typename T>
using FPDeleter = void (*)(T*);

/** \brief Unique pointer with a function pointer deleter, useful for externally managed pointers. */
template <typename T>
using UniqueForeign = Unique<T, FPDeleter<T>>;

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

using std::make_shared;
using std::make_unique;

template <typename T>
[[nodiscard]] auto make_unique_foreign(T* ptr, FPDeleter<T> deleter) -> UniqueForeign<T>
{
  return UniqueForeign<T> {ptr, deleter};
}

}  // namespace tactile
