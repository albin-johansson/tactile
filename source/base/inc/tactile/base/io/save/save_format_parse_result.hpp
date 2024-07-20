// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/io/save/save_format_parse_error.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

template <typename T>
using SaveFormatParseResult = Expected<T, SaveFormatParseError>;

inline constexpr SaveFormatParseResult<void> kParseResultOK {};

}  // namespace tactile
