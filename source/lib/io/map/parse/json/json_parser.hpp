// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/parse_error.hpp"
#include "io/map/parse/parse_result.hpp"
#include "io/util/json.hpp"
#include "tactile/base/container/path.hpp"

namespace tactile {

[[nodiscard]] auto parse_json_map(const Path& path) -> ParseResult;

[[nodiscard]] auto parse_tilesets(const JSON& json, MapIR& map, const Path& dir)
    -> ParseError;

[[nodiscard]] auto parse_layers(const JSON& json, MapIR& map) -> ParseError;

[[nodiscard]] auto parse_object(const JSON& json, ObjectIR& object) -> ParseError;

[[nodiscard]] auto parse_properties(const JSON& json, ContextIR& context) -> ParseError;

}  // namespace tactile
