// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu_bar.hpp"

#include <imgui.h>

namespace tactile::core {

void MenuBar::update(const Model& model, EventDispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    ImGui::EndMainMenuBar();
  }
}

}  // namespace tactile::core
