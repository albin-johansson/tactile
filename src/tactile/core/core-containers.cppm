// Copyright (C) 2025 Albin Johansson

export module tactile.core:containers;

export import std;
export import :primitives;

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

template <typename T>
using Unexpected = std::unexpected<T>;

inline constexpr std::nullopt_t kNone {std::nullopt};

}  // namespace tactile
