// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/style.hpp"

namespace tactile::ui {

StyleVarScope::StyleVarScope(const ImGuiStyleVar id, const ImVec2 value)
{
  ImGui::PushStyleVar(id, value);
}

StyleVarScope::StyleVarScope(const ImGuiStyleVar id, const float value)
{
  ImGui::PushStyleVar(id, value);
}

StyleVarScope::~StyleVarScope() noexcept
{
  pop();
}

void StyleVarScope::pop() noexcept
{
  if (!mPopped) {
    ImGui::PopStyleVar();
    mPopped = true;
  }
}

StyleColorScope::StyleColorScope(const ImGuiCol id, const ImVec4& color)
{
  ImGui::PushStyleColor(id, color);
}

StyleColorScope::StyleColorScope(const ImGuiCol id, const uint32 color)
{
  ImGui::PushStyleColor(id, color);
}

StyleColorScope::~StyleColorScope() noexcept
{
  pop();
}

void StyleColorScope::pop() noexcept
{
  if (!mPopped) {
    ImGui::PopStyleColor();
    mPopped = true;
  }
}

}  // namespace tactile::ui
