// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "map_parser.hpp"

namespace tactile {

auto parse_map_with_yaml_format(const Path& path) -> ParseResult
{
  return unexpected(ParseError::Unknown);
}

auto parse_map_with_tiled_tmj_format(const Path& path) -> ParseResult
{
  return unexpected(ParseError::Unknown);
}

auto parse_map_with_tiled_tmx_format(const Path& path) -> ParseResult
{
  return unexpected(ParseError::Unknown);
}

}  // namespace tactile
