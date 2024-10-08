// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include "tactile/base/debug/error_code.hpp"

namespace tactile {

template <typename T>
using Result = std::expected<T, ErrorCode>;

inline constexpr Result<void> kOK {};

}  // namespace tactile
