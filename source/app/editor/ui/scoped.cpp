/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "scoped.hpp"

#include "misc/panic.hpp"

namespace tactile::ui {

Scope::Scope(const void* ptr)
{
  ImGui::PushID(ptr);
}

Scope::Scope(const char* str)
{
  ImGui::PushID(str);
}

Scope::Scope(const UUID& id)
{
  ImGui::PushID(static_cast<int>(hash(id)));
}

Scope::Scope(const int id)
{
  ImGui::PushID(id);
}

Scope::~Scope()
{
  ImGui::PopID();
}

Disable::Disable(const bool disable)
{
  ImGui::BeginDisabled(disable);
}

Disable::~Disable()
{
  ImGui::EndDisabled();
}

Indent::Indent()
{
  ImGui::Indent();
}

Indent::~Indent()
{
  ImGui::Unindent();
}

Tooltip::Tooltip()
{
  ImGui::BeginTooltip();
}

Tooltip::~Tooltip()
{
  ImGui::EndTooltip();
}

StyleVar::StyleVar(const ImGuiStyleVar index, const ImVec2& value)
{
  ImGui::PushStyleVar(index, value);
}

StyleVar::StyleVar(const ImGuiStyleVar index, const float value)
{
  ImGui::PushStyleVar(index, value);
}

StyleVar::~StyleVar()
{
  pop();
}

void StyleVar::pop()
{
  if (!mPopped) {
    ImGui::PopStyleVar();
    mPopped = true;
  }
}

StyleColor::StyleColor(const ImGuiCol index, const ImVec4& color)
{
  ImGui::PushStyleColor(index, color);
}

StyleColor::StyleColor(const ImGuiCol index, const uint32 value)
{
  ImGui::PushStyleColor(index, value);
}

StyleColor::~StyleColor()
{
  pop();
}

void StyleColor::pop()
{
  if (!mPopped) {
    ImGui::PopStyleColor();
    mPopped = true;
  }
}

Group::Group()
{
  ImGui::BeginGroup();
}

Group::~Group()
{
  ImGui::EndGroup();
}

Child::Child(const char* id,
             const ImVec2& size,
             const bool border,
             const ImGuiWindowFlags flags)
    : mOpen {ImGui::BeginChild(id, size, border, flags)}
{
}

Child::~Child()
{
  ImGui::EndChild();
}

Combo::Combo(const char* name, const char* current)
    : mOpen {ImGui::BeginCombo(name, current)}
{
}

Combo::~Combo()
{
  if (mOpen) {
    ImGui::EndCombo();
  }
}

TabBar::TabBar(const char* name, const ImGuiTabBarFlags flags)
    : mOpen {ImGui::BeginTabBar(name, flags)}
{
}

TabBar::~TabBar()
{
  if (mOpen) {
    ImGui::EndTabBar();
  }
}

TabItem::TabItem(const char* name, bool* open, const ImGuiTabItemFlags flags)
    : mOpen {ImGui::BeginTabItem(name, open, flags)}
{
}

TabItem::~TabItem()
{
  if (mOpen) {
    ImGui::EndTabItem();
  }
}

Table::Table(const char* name, const int column_count, const ImGuiTableFlags flags)
    : mOpen {ImGui::BeginTable(name, column_count, flags)}
{
}

Table::~Table()
{
  if (mOpen) {
    ImGui::EndTable();
  }
}

Popup::Popup(const bool open)
    : mOpen {open}
{
}

Popup::Popup(const char* name, const ImGuiWindowFlags flags)
    : mOpen {ImGui::BeginPopup(name, flags)}
{
}

Popup::~Popup()
{
  if (mOpen) {
    ImGui::EndPopup();
  }
}

auto Popup::for_item(const char* name, const ImGuiPopupFlags flags) -> Popup
{
  return Popup {ImGui::BeginPopupContextItem(name, flags)};
}

auto Popup::for_window(const char* name, const ImGuiPopupFlags flags) -> Popup
{
  return Popup {ImGui::BeginPopupContextWindow(name, flags)};
}

ListBox::ListBox(const char* label, const ImVec2& size)
    : mOpen {ImGui::BeginListBox(label, size)}
{
}

ListBox::~ListBox()
{
  if (mOpen) {
    ImGui::EndListBox();
  }
}

Menu::Menu(const char* name, const bool enabled)
    : mOpen {ImGui::BeginMenu(name, enabled)}
{
}

Menu::~Menu()
{
  if (mOpen) {
    ImGui::EndMenu();
  }
}

Modal::Modal(const char* name, const ImGuiWindowFlags flags, bool* open)
    : mOpen {ImGui::BeginPopupModal(name, open, flags)}
{
}

Modal::~Modal()
{
  if (mOpen) {
    ImGui::EndPopup();
  }
}

Window::Window(const char* label, const ImGuiWindowFlags flags, bool* open)
    : mLabel {label}
    , mOpen {ImGui::Begin(label, open, flags)}
{
  if (!label) {
    throw TactileError {"Invalid null window label!"};
  }

  auto& data = window_data[label];
  data.was_hovered = data.is_hovered;
  data.is_hovered = is_hovered();
}

Window::~Window()
{
  ImGui::End();
}

auto Window::has_focus(const ImGuiFocusedFlags flags) const -> bool
{
  return mOpen && ImGui::IsWindowFocused(flags);
}

auto Window::mouse_entered() const -> bool
{
  const auto& data = window_data.at(mLabel);
  return !data.was_hovered && data.is_hovered;
}

auto Window::mouse_exited() const -> bool
{
  const auto& data = window_data.at(mLabel);
  return data.was_hovered && !data.is_hovered;
}

auto Window::contains_mouse() -> bool
{
  const auto pos = ImGui::GetWindowPos();
  const auto size = ImGui::GetWindowSize();
  const auto max = ImVec2 {pos.x + size.x, pos.y + size.y};
  return ImGui::IsMouseHoveringRect(pos, max);
}

auto Window::is_hovered() const -> bool
{
  return mOpen && contains_mouse();
}

TreeNode::TreeNode(const char* id, const ImGuiTreeNodeFlags flags)
    : mOpen {ImGui::TreeNodeEx(id, flags)}
{
}

TreeNode::~TreeNode()
{
  if (mOpen) {
    ImGui::TreePop();
  }
}

}  // namespace tactile::ui