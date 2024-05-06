// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/widgets.hpp"

#include "tactile/core/numeric/narrow.hpp"

namespace tactile::ui {

IdScope::IdScope(const void* id)
{
  ImGui::PushID(id);
}

IdScope::IdScope(const char* id)
{
  ImGui::PushID(id);
}

IdScope::IdScope(const int id)
{
  ImGui::PushID(id);
}

IdScope::IdScope(const EntityID id)
{
  ImGui::PushID(narrow_cast<int>(id));
}

IdScope::~IdScope() noexcept
{
  ImGui::PopID();
}

DisabledScope::DisabledScope(const bool disabled)
{
  ImGui::BeginDisabled(disabled);
}

DisabledScope::~DisabledScope() noexcept
{
  ImGui::EndDisabled();
}

IndentScope::IndentScope()
{
  ImGui::Indent();
}

IndentScope::~IndentScope() noexcept
{
  ImGui::Unindent();
}

GroupScope::GroupScope()
{
  ImGui::BeginGroup();
}

GroupScope::~GroupScope() noexcept
{
  ImGui::EndGroup();
}

TooltipScope::TooltipScope()
{
  ImGui::BeginTooltip();
}

TooltipScope::~TooltipScope() noexcept
{
  ImGui::EndTooltip();
}

}  // namespace tactile::ui
