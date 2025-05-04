// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.core:strings;

export import std;
export import :containers;
export import :primitives;

export namespace tactile {

using String = std::string;

using StringView = std::string_view;

/// Parses an `i64` from a given string.
[[nodiscard]]
auto parse_i64(StringView str, int base = 10) noexcept -> Option<i64>;

/// Parses a `u64` from a given string.
[[nodiscard]]
auto parse_u64(StringView str, int base = 10) noexcept -> Option<u64>;

/// Parses an `f64` from a given string.
[[nodiscard]]
auto parse_f64(StringView str) noexcept -> Option<f64>;

}  // namespace tactile
