// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile::core {

struct CViewport;

namespace ui {

class CanvasRenderer;

void push_viewport_info_section(const CViewport& viewport);

void push_canvas_info_section(const CanvasRenderer& canvas_renderer);

void push_viewport_mouse_info_section(const CanvasRenderer& canvas_renderer);

}  // namespace ui
}  // namespace tactile::core
