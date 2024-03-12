// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "scoped.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"
#include "ui/conversions.hpp"
#include "ui/style/colors.hpp"

namespace tactile::ui {
namespace {

inline constexpr ImVec4 kWhiteVec4 {0xFF, 0xFF, 0xFF, 0xFF};
inline constexpr ImVec4 kBlackVec4 {0x00, 0x00, 0x00, 0xFF};

[[nodiscard]] auto get_text_fg(const ImGuiCol bg) -> ImVec4
{
  return is_dark(bg) ? kWhiteVec4 : kBlackVec4;
}

[[nodiscard]] auto get_tab_item_text_color(const ImGuiTabItemFlags flags) -> ImVec4
{
  return get_text_fg((flags & ImGuiTabItemFlags_SetSelected) ? ImGuiCol_TabActive
                                                             : ImGuiCol_Tab);
}

[[nodiscard]] auto get_tree_node_text_color() -> ImVec4
{
  return get_text_fg(ImGuiCol_WindowBg);
}

[[nodiscard]] auto get_selectable_list_item_text_color(const bool selected) -> ImVec4
{
  return get_text_fg(selected ? ImGuiCol_HeaderActive : ImGuiCol_FrameBg);
}

[[nodiscard]] auto get_selectable_property_text_color(const bool selected) -> ImVec4
{
  return get_text_fg(selected ? ImGuiCol_ChildBg : ImGuiCol_WindowBg);
}

[[nodiscard]] auto get_window_label_text_color(const WindowData& data) -> ImVec4
{
  if (data.is_docked) {
    if (data.has_focus) {
      return get_text_fg(data.is_open ? ImGuiCol_TabActive : ImGuiCol_Tab);
    }
    else {
      return get_text_fg(data.is_open ? ImGuiCol_TabUnfocusedActive
                                      : ImGuiCol_TabUnfocused);
    }
  }
  else {
    return get_text_fg(data.has_focus ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg);
  }
}

}  // namespace

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
  ImGui::PushID(static_cast<int>(id.hash_code()));
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
    : mTextColor {ImGuiCol_Text, get_tab_item_text_color(flags)},
      mOpen {ImGui::BeginTabItem(name, open, flags)}
{
  mTextColor.pop();
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

auto Selectable::list_item(const char* label,
                           const bool selected,
                           const ImGuiSelectableFlags flags,
                           const ImVec2& size) -> bool
{
  StyleColor text_color {ImGuiCol_Text, get_selectable_list_item_text_color(selected)};

  const auto activated = ImGui::Selectable(label, selected, flags, size);
  text_color.pop();

  return activated;
}

auto Selectable::property(const char* label,
                          const bool selected,
                          const ImGuiSelectableFlags flags,
                          const ImVec2& size) -> bool
{
  StyleColor text_color {ImGuiCol_Text, get_selectable_property_text_color(selected)};

  const auto activated = ImGui::Selectable(label, selected, flags, size);
  text_color.pop();

  return activated;
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
    : mTextColor {ImGuiCol_Text, get_window_label_text_color(mWindowData[label])},
      mLabel {label},
      mOpen {ImGui::Begin(label, open, flags)}
{
  if (!label) {
    throw Exception {"Invalid null window label!"};
  }

  mTextColor.pop();

  auto& data = mWindowData[label];
  data.has_focus = has_focus(ImGuiFocusedFlags_RootAndChildWindows);
  data.was_hovered = data.is_hovered;
  data.is_hovered = is_hovered();
  data.is_docked = ImGui::IsWindowDocked();
  data.is_open = mOpen;
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
  const auto& data = mWindowData.at(mLabel);
  return !data.was_hovered && data.is_hovered;
}

auto Window::mouse_exited() const -> bool
{
  const auto& data = mWindowData.at(mLabel);
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
    : mTextColor {ImGuiCol_Text, get_tree_node_text_color()},
      mOpen {ImGui::TreeNodeEx(id, flags)}
{
  mTextColor.pop();
}

TreeNode::TreeNode(const char* id, ImGuiTreeNodeFlags flags, const char* label)

    : mTextColor {ImGuiCol_Text, get_tree_node_text_color()},
      mOpen {ImGui::TreeNodeEx(id, flags, "%s", label)}
{
  mTextColor.pop();
}

TreeNode::~TreeNode()
{
  if (mOpen) {
    ImGui::TreePop();
  }
}

}  // namespace tactile::ui
