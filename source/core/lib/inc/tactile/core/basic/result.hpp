// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected, unexpected

#include "tactile/core/basic/errc.hpp"

namespace tactile {

template <typename T>
using Result = std::expected<T, Errc>;

[[nodiscard]]
constexpr auto error(const Errc errc) noexcept -> std::unexpected<Errc>
{
  return std::unexpected {errc};
}

}  // namespace tactile
