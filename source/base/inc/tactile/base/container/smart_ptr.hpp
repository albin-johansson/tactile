// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr, shared_ptr, weak_ptr, default_delete

namespace tactile {

template <typename T, typename Deleter = std::default_delete<T>>
using Unique = std::unique_ptr<T, Deleter>;

template <typename T>
using UniqueForeign = Unique<T, void (*)(T*) noexcept>;

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

}  // namespace tactile
