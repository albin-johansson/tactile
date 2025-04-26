// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <array>
#include <deque>
#include <expected>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <unordered_map>
#include <vector>

export module tactile.core:containers;

import :primitives;

export namespace tactile {

template <typename T, usize Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

template <typename T, usize N>
using Array = std::array<T, N>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Deque = std::deque<T>;

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

template <typename K, typename V>
using TreeMap = std::map<K, V, std::less<>>;

template <typename T>
using Option = std::optional<T>;

template <typename T, typename E>
using Expected = std::expected<T, E>;

using std::nullopt;

}  // namespace tactile
