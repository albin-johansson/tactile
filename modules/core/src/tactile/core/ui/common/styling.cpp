// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/styling.hpp"

namespace tactile::ui {

ScopedStyle::ScopedStyle(const ImGuiStyleVar style_var, const float value)
{
  ImGui::PushStyleVar(style_var, value);
}

ScopedStyle::ScopedStyle(const ImGuiStyleVar style_var, const ImVec2& value)
{
  ImGui::PushStyleVar(style_var, value);
}

ScopedStyle::~ScopedStyle() noexcept
{
  ImGui::PopStyleVar();
}

}  // namespace tactile::ui
