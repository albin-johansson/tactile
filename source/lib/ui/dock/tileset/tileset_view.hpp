// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)

namespace tactile::ui {

void update_tileset_view(const DocumentModel& model,
                         const UUID& tileset_id,
                         entt::dispatcher& dispatcher);

}  // namespace tactile::ui
