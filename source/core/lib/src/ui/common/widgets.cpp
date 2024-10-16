// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/widgets.hpp"

#include <utility>  // to_underlying

#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/ui/imgui_compat.hpp"

namespace tactile::core::ui {

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
  ImGui::PushID(saturate_cast<int>(std::to_underlying(id)));
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

ComboScope::ComboScope(const char* label, const char* preview, const ImGuiComboFlags flags)
  : mIsOpen {ImGui::BeginCombo(label, preview, flags)}
{}

ComboScope::~ComboScope() noexcept
{
  if (mIsOpen) {
    ImGui::EndCombo();
  }
}

auto ComboScope::is_open() const -> bool
{
  return mIsOpen;
}

TableScope::TableScope(const char* label, const int columns, const ImGuiTableFlags flags)
  : mIsOpen {ImGui::BeginTable(label, columns, flags)}
{}

TableScope::~TableScope() noexcept
{
  if (mIsOpen) {
    ImGui::EndTable();
  }
}

auto TableScope::is_open() const -> bool
{
  return mIsOpen;
}

TabBarScope::TabBarScope(const char* id, const ImGuiTabBarFlags flags)
  : mIsOpen {ImGui::BeginTabBar(id, flags)}
{}

TabBarScope::~TabBarScope() noexcept
{
  if (mIsOpen) {
    ImGui::EndTabBar();
  }
}

auto TabBarScope::is_open() const -> bool
{
  return mIsOpen;
}

TabItemScope::TabItemScope(const char* label, const ImGuiTabItemFlags flags)
  : mIsOpen {ImGui::BeginTabItem(label, nullptr, flags)}
{}

TabItemScope::~TabItemScope() noexcept
{
  if (mIsOpen) {
    ImGui::EndTabItem();
  }
}

auto TabItemScope::is_open() const -> bool
{
  return mIsOpen;
}

auto get_widget_size(const char* text) -> Float2
{
  TACTILE_ASSERT(text != nullptr);
  return to_float2(ImGui::CalcTextSize(text) + (ImGui::GetStyle().FramePadding * 2.0f));
}

void center_next_widget_horizontally(const float width)
{
  const auto half_content_region = ImGui::GetContentRegionAvail() * 0.5f;
  const ImVec2 cursor_offset {half_content_region.x - (width * 0.5f), 0};
  ImGui::SetCursorPos(ImGui::GetCursorPos() + cursor_offset);
}

void center_next_widget_vertically(const float height)
{
  const auto half_content_region = ImGui::GetContentRegionAvail() * 0.5f;
  const ImVec2 cursor_offset {0, half_content_region.y - (height * 0.5f)};
  ImGui::SetCursorPos(ImGui::GetCursorPos() + cursor_offset);
}

void center_next_widget(const Float2& size)
{
  center_next_widget_horizontally(size.x());
  center_next_widget_vertically(size.y());
}

void prepare_for_vertically_centered_widgets(const float count)
{
  center_next_widget_vertically(count * ImGui::GetFrameHeight());
}

void push_centered_label(const char* text)
{
  TACTILE_ASSERT(text != nullptr);
  const auto text_size = ImGui::CalcTextSize(text);
  center_next_widget_horizontally(text_size.x);
  ImGui::TextUnformatted(text);
}

}  // namespace tactile::core::ui
