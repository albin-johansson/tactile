// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/signal/fwd.hpp>

struct ImVec2;

namespace tactile::ui {

void update_document_viewport_offset(const ImVec2& viewport_size,
                                     entt::dispatcher& dispatcher);

}  // namespace tactile::ui
