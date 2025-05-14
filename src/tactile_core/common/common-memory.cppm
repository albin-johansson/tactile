// Copyright (C) 2025 Albin Johansson

export module tactile.common:memory;

export import std;

export namespace tactile {

template <typename T, typename Deleter = std::default_delete<T>>
using Unique = std::unique_ptr<T, Deleter>;

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

using std::make_shared;
using std::make_unique;

}  // namespace tactile
