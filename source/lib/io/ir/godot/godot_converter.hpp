// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/ir/godot/godot_scene.hpp"
#include "io/ir/map/map_ir.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(GodotEmitOptions)

[[nodiscard]] auto convert_to_godot(const MapIR& map, const GodotEmitOptions& options)
    -> GodotScene;

}  // namespace tactile
