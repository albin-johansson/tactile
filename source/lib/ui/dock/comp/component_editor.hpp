// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/signal/fwd.hpp>

#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)

namespace tactile::ui {

void open_component_editor_dialog(const DocumentModel& model);

void update_component_editor_dialog(const DocumentModel& model,
                                    entt::dispatcher& dispatcher);

}  // namespace tactile::ui
