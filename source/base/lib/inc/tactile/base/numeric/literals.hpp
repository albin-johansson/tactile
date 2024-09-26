// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t, ptrdiff_t

namespace tactile {

[[nodiscard]]
consteval auto operator""_uz(const unsigned long long int value) noexcept -> std::size_t
{
  return static_cast<std::size_t>(value);
}

[[nodiscard]]
consteval auto operator""_z(const unsigned long long int value) noexcept -> std::ptrdiff_t
{
  return static_cast<std::ptrdiff_t>(value);
}

}  // namespace tactile
