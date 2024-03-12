// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/signal/fwd.hpp>

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)

namespace tactile::ui {

void open_rename_property_dialog(const UUID& context_id, String previous_name);

void update_rename_property_dialog(const DocumentModel& model,
                                   entt::dispatcher& dispatcher);

}  // namespace tactile::ui
