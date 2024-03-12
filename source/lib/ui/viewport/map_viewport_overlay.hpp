// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Map)

namespace tactile::ui {

TACTILE_FWD_DECLARE_STRUCT(ViewportCursorInfo)

void update_map_viewport_overlay(const Map& map, const ViewportCursorInfo& cursor);

}  // namespace tactile::ui
