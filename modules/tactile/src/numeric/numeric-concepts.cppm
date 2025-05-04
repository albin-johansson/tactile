// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <concepts>

export module tactile.numeric:concepts;

export namespace tactile {

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

}  // namespace tactile
