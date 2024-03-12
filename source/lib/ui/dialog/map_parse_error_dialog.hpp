// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/map/parse/parse_error.hpp"

namespace tactile::ui {

void open_map_parse_error_dialog(ParseError error);
void update_map_parse_error_dialog();

}  // namespace tactile::ui
