// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/signal/fwd.hpp>

#include "tactile/base/container/string.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile::ui {

void open_rename_layer_dialog(const UUID& layer_id, String current_name);
void update_rename_layer_dialog(entt::dispatcher& dispatcher);

}  // namespace tactile::ui
