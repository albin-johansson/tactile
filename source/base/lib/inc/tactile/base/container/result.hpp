// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

namespace tactile {

template <typename T>
using Result = std::expected<T, std::error_code>;

inline const Result<void> kOK {};

}  // namespace tactile
