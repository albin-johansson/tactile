// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/menu_bar.hpp"

#include <imgui.h>

namespace tactile::ui {

void MenuBar::push(const Model& model, EventDispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    mFileMenu.push(model, dispatcher);
    mEditMenu.push(model, dispatcher);
    mViewMenu.push(model, dispatcher);
    mMapMenu.push(model, dispatcher);
    mTilesetMenu.push(model, dispatcher);
    mHelpMenu.push(model, dispatcher);
    mDebugMenu.push(model);

    ImGui::EndMainMenuBar();
  }
}

}  // namespace tactile::ui
