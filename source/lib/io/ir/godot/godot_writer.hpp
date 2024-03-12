// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(GodotScene)
TACTILE_FWD_DECLARE_STRUCT(GodotEmitOptions)

void write_godot_scene(const GodotScene& scene, const GodotEmitOptions& options);

}  // namespace tactile
