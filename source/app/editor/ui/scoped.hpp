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

#pragma once

#include <utility>  // forward

#include <imgui.h>

#include "core/common/associative.hpp"
#include "core/common/ints.hpp"
#include "core/common/macros.hpp"

namespace tactile::ui {

struct Scope final
{
  TACTILE_DEFAULT_COPY(Scope);
  TACTILE_DEFAULT_MOVE(Scope);

  explicit Scope(const void* ptr) { ImGui::PushID(ptr); }

  explicit Scope(const char* str) { ImGui::PushID(str); }

  explicit Scope(const int id) { ImGui::PushID(id); }

  ~Scope() { ImGui::PopID(); }
};

struct Disable final
{
  TACTILE_DEFAULT_COPY(Disable);
  TACTILE_DEFAULT_MOVE(Disable);

  explicit Disable(const bool disable = true) { ImGui::BeginDisabled(disable); }

  ~Disable() { ImGui::EndDisabled(); }
};

struct Tooltip final
{
  TACTILE_DEFAULT_COPY(Tooltip);
  TACTILE_DEFAULT_MOVE(Tooltip);

  Tooltip() { ImGui::BeginTooltip(); }

  ~Tooltip() { ImGui::EndTooltip(); }
};

class StyleVar final
{
 public:
  TACTILE_DEFAULT_COPY(StyleVar);
  TACTILE_DEFAULT_MOVE(StyleVar);

  StyleVar(const ImGuiStyleVar index, const ImVec2& value)
  {
    ImGui::PushStyleVar(index, value);
  }

  StyleVar(const ImGuiStyleVar index, const float value)
  {
    ImGui::PushStyleVar(index, value);
  }

  ~StyleVar() { pop(); }

  void pop()
  {
    if (!mPopped) {
      ImGui::PopStyleVar();
      mPopped = true;
    }
  }

 private:
  bool mPopped{};
};

class StyleColor final
{
 public:
  TACTILE_DEFAULT_COPY(StyleColor);
  TACTILE_DEFAULT_MOVE(StyleColor);

  StyleColor(const ImGuiCol index, const ImVec4& color)
  {
    ImGui::PushStyleColor(index, color);
  }

  StyleColor(const ImGuiCol index, const uint32 value)
  {
    ImGui::PushStyleColor(index, value);
  }

  ~StyleColor() { pop(); }

  void pop()
  {
    if (!mPopped) {
      ImGui::PopStyleColor();
      mPopped = true;
    }
  }

 private:
  bool mPopped{};
};

struct Group final
{
  TACTILE_DEFAULT_COPY(Group);
  TACTILE_DEFAULT_MOVE(Group);

  Group() { ImGui::BeginGroup(); }

  ~Group() { ImGui::EndGroup(); }
};

class Child final
{
 public:
  TACTILE_DEFAULT_COPY(Child);
  TACTILE_DEFAULT_MOVE(Child);

  explicit Child(const char* id,
                 const ImVec2& size = {0, 0},
                 const bool border = false,
                 const ImGuiWindowFlags flags = 0)
      : mOpen{ImGui::BeginChild(id, size, border, flags)}
  {}

  ~Child() { ImGui::EndChild(); }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Combo final
{
 public:
  TACTILE_DEFAULT_COPY(Combo);
  TACTILE_DEFAULT_MOVE(Combo);

  Combo(const char* name, const char* current) : mOpen{ImGui::BeginCombo(name, current)}
  {}

  ~Combo()
  {
    if (mOpen) {
      ImGui::EndCombo();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class TabBar final
{
 public:
  TACTILE_DEFAULT_COPY(TabBar);
  TACTILE_DEFAULT_MOVE(TabBar);

  explicit TabBar(const char* name, const ImGuiTabBarFlags flags = 0)
      : mOpen{ImGui::BeginTabBar(name, flags)}
  {}

  ~TabBar()
  {
    if (mOpen) {
      ImGui::EndTabBar();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class TabItem final
{
 public:
  TACTILE_DEFAULT_COPY(TabItem);
  TACTILE_DEFAULT_MOVE(TabItem);

  explicit TabItem(const char* name,
                   bool* open = nullptr,
                   const ImGuiTabItemFlags flags = 0)
      : mOpen{ImGui::BeginTabItem(name, open, flags)}
  {}

  ~TabItem()
  {
    if (mOpen) {
      ImGui::EndTabItem();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Table final
{
 public:
  TACTILE_DEFAULT_COPY(Table);
  TACTILE_DEFAULT_MOVE(Table);

  Table(const char* name, const int nColumns, const ImGuiTableFlags flags = 0)
      : mOpen{ImGui::BeginTable(name, nColumns, flags)}
  {}

  ~Table()
  {
    if (mOpen) {
      ImGui::EndTable();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Popup final
{
 public:
  TACTILE_DEFAULT_COPY(Popup);
  TACTILE_DEFAULT_MOVE(Popup);

  explicit Popup(const char* name, const ImGuiWindowFlags flags = 0)
      : mOpen{ImGui::BeginPopup(name, flags)}
  {}

  ~Popup()
  {
    if (mOpen) {
      ImGui::EndPopup();
    }
  }

  [[nodiscard]] static auto for_item(const char* name, const ImGuiPopupFlags flags = 1)
      -> Popup
  {
    return Popup{ImGui::BeginPopupContextItem(name, flags)};
  }

  [[nodiscard]] static auto for_window(const char* name, const ImGuiPopupFlags flags = 1)
      -> Popup
  {
    return Popup{ImGui::BeginPopupContextWindow(name, flags)};
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};

  explicit Popup(const bool open) : mOpen{open} {}
};

class ListBox final
{
 public:
  TACTILE_DEFAULT_COPY(ListBox);
  TACTILE_DEFAULT_MOVE(ListBox);

  explicit ListBox(const char* label, const ImVec2& size = {0, 0})
      : mOpen{ImGui::BeginListBox(label, size)}
  {}

  ~ListBox()
  {
    if (mOpen) {
      ImGui::EndListBox();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Menu final
{
 public:
  TACTILE_DEFAULT_COPY(Menu);
  TACTILE_DEFAULT_MOVE(Menu);

  explicit Menu(const char* name, const bool enabled = true)
      : mOpen{ImGui::BeginMenu(name, enabled)}
  {}

  ~Menu()
  {
    if (mOpen) {
      ImGui::EndMenu();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Modal final
{
 public:
  TACTILE_DEFAULT_COPY(Modal);
  TACTILE_DEFAULT_MOVE(Modal);

  explicit Modal(const char* name, const ImGuiWindowFlags flags = 0, bool* open = nullptr)
      : mOpen{ImGui::BeginPopupModal(name, open, flags)}
  {}

  ~Modal()
  {
    if (mOpen) {
      ImGui::EndPopup();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Window final
{
  struct WindowData final
  {
    bool was_hovered : 1 {};
    bool is_hovered : 1 {};
  };

  inline static HashMap<const char*, WindowData> window_data;

 public:
  TACTILE_DEFAULT_COPY(Window);
  TACTILE_DEFAULT_MOVE(Window);

  explicit Window(const char* label,
                  const ImGuiWindowFlags flags = 0,
                  bool* open = nullptr)
      : mLabel{label}
      , mOpen{ImGui::Begin(label, open, flags)}
  {
    auto& data = window_data[label];
    data.was_hovered = data.is_hovered;
    data.is_hovered = is_hovered();
  }

  ~Window() { ImGui::End(); }

  [[nodiscard]] auto has_focus(const ImGuiFocusedFlags flags = 0) const -> bool
  {
    return mOpen && ImGui::IsWindowFocused(flags);
  }

  [[nodiscard]] auto mouse_entered() const noexcept -> bool
  {
    const auto& data = window_data.at(mLabel);
    return !data.was_hovered && data.is_hovered;
  }

  [[nodiscard]] auto mouse_exited() const noexcept -> bool
  {
    const auto& data = window_data.at(mLabel);
    return data.was_hovered && !data.is_hovered;
  }

  [[nodiscard]] static auto contains_mouse() -> bool
  {
    const auto pos = ImGui::GetWindowPos();
    const auto size = ImGui::GetWindowSize();
    const auto max = ImVec2{pos.x + size.x, pos.y + size.y};
    return ImGui::IsMouseHoveringRect(pos, max);
  }

  [[nodiscard]] auto is_hovered() const -> bool { return mOpen && contains_mouse(); }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  const char* mLabel{};
  bool mOpen{};
};

class TreeNode final
{
 public:
  TACTILE_DEFAULT_COPY(TreeNode);
  TACTILE_DEFAULT_MOVE(TreeNode);

  explicit TreeNode(const char* id, const ImGuiTreeNodeFlags flags = 0)
      : mOpen{ImGui::TreeNodeEx(id, flags)}
  {}

  template <typename... Args>
  explicit TreeNode(const char* id,
                    const ImGuiTreeNodeFlags flags,
                    const char* fmt,
                    Args&&... args)
      : mOpen{ImGui::TreeNodeEx(id, flags, fmt, std::forward<Args>(args)...)}
  {}

  ~TreeNode()
  {
    if (mOpen) {
      ImGui::TreePop();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

}  // namespace tactile::ui