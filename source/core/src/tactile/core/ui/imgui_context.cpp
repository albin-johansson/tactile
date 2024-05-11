// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/imgui_context.hpp"

#include <imgui.h>

namespace tactile::ui {

void ImGuiContextDeleter::operator()(ImGuiContext* context) noexcept
{
  ImGui::DestroyContext(context);
}

}  // namespace tactile::ui
