// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/ir/map/map_ir.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(MapDocument)

[[nodiscard]] auto convert_map_document_to_ir(const MapDocument& document) -> MapIR;

}  // namespace tactile
