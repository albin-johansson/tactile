// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/map/parse/parse_result.hpp"
#include "tactile/base/container/path.hpp"

namespace tactile {

[[nodiscard]] auto parse_map_with_yaml_format(const Path& path) -> ParseResult;

[[nodiscard]] auto parse_map_with_tiled_tmj_format(const Path& path) -> ParseResult;
[[nodiscard]] auto parse_map_with_tiled_tmx_format(const Path& path) -> ParseResult;

}  // namespace tactile
