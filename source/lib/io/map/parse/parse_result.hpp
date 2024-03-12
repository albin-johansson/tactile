// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/parse_error.hpp"
#include "tactile/base/container/expected.hpp"

namespace tactile {

using ParseResult = Expected<MapIR, ParseError>;

}  // namespace tactile
