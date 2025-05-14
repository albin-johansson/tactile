// Copyright (C) 2025 Albin Johansson

export module tactile.core.numeric:concepts;

export import std;

export namespace tactile {

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

}  // namespace tactile
