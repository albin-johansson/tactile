// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock_space.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/core/model/model.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

void DockSpace::update(const Model&, EventDispatcher&)
{
  const auto* viewport = require_not_null(ImGui::GetMainViewport(), "null main viewport");
  mRootId = ImGui::DockSpaceOverViewport(viewport);

  if (!mDidInit && viewport->Size.x > 0 && viewport->Size.y > 0) {
    TACTILE_LOG_DEBUG("[DockSpace] Initializing dock space");

    // TODO

    mDidInit = true;
  }
}

void DockSpace::_load_default_layout()
{
  TACTILE_LOG_DEBUG("[DockSpace] Loading default layout");
}

}  // namespace tactile::core
