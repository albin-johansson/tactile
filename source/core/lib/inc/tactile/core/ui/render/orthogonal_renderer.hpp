// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/ui/canvas_renderer.hpp"

namespace tactile {

class Registry;

namespace ui {

class CanvasRenderer;

void render_orthogonal_map(const CanvasRenderer& canvas_renderer,
                           const Registry& registry,
                           EntityID map_id);

}  // namespace ui
}  // namespace tactile
