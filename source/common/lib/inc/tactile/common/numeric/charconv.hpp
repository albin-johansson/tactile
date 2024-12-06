// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace tactile::common {

/// Parses a signed integer from a given string.
[[nodiscard]]
auto parse_int(std::string_view str, int base = 10) noexcept -> std::optional<std::int64_t>;

/// Parses an unsigned integer from a given string.
[[nodiscard]]
auto parse_uint(std::string_view str, int base = 10) noexcept -> std::optional<std::uint64_t>;

/// Parses a float from a given string.
[[nodiscard]]
auto parse_double(std::string_view str) noexcept -> std::optional<double>;

}  // namespace tactile::common
