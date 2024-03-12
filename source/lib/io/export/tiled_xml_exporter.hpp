// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(MapIR)

void save_map_as_tiled_xml(const Path& destination, const MapIR& ir_map);

}  // namespace tactile
