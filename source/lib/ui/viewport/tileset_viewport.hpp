// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/signal/fwd.hpp>

#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::ui {

void show_tileset_viewport(const TilesetDocument& document, entt::dispatcher& dispatcher);

void center_tileset_viewport();

void enable_tile_animation_frame_selection_mode();

}  // namespace tactile::ui
