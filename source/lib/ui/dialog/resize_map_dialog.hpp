// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/signal/fwd.hpp>

#include "core/tile/tile_extent.hpp"

namespace tactile::ui {

void open_resize_map_dialog(TileExtent current_extent);
void update_resize_map_dialog(entt::dispatcher& dispatcher);

}  // namespace tactile::ui
