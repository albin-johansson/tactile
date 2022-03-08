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

#include "tactile.hpp"

namespace tactile::scoped {

struct id final
{
  TACTILE_DEFAULT_COPY(id)
  TACTILE_DEFAULT_MOVE(id)

  explicit id(const void* ptr) { ImGui::PushID(ptr); }

  explicit id(const char* str) { ImGui::PushID(str); }

  explicit id(const int id) { ImGui::PushID(id); }

  ~id() { ImGui::PopID(); }
};

struct disable final
{
  TACTILE_DEFAULT_COPY(disable)
  TACTILE_DEFAULT_MOVE(disable)

  explicit disable(const bool disable = true) { ImGui::BeginDisabled(disable); }

  ~disable() { ImGui::EndDisabled(); }
};

struct tooltip final
{
  TACTILE_DEFAULT_COPY(tooltip)
  TACTILE_DEFAULT_MOVE(tooltip)

  tooltip() { ImGui::BeginTooltip(); }

  ~tooltip() { ImGui::EndTooltip(); }
};

class style_var final
{
 public:
  TACTILE_DEFAULT_COPY(style_var)
  TACTILE_DEFAULT_MOVE(style_var)

  style_var(const ImGuiStyleVar index, const ImVec2& value)
  {
    ImGui::PushStyleVar(index, value);
  }

  style_var(const ImGuiStyleVar index, const float value)
  {
    ImGui::PushStyleVar(index, value);
  }

  ~style_var() { pop(); }

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

class style_color final
{
 public:
  TACTILE_DEFAULT_COPY(style_color)
  TACTILE_DEFAULT_MOVE(style_color)

  style_color(const ImGuiCol index, const ImVec4& color)
  {
    ImGui::PushStyleColor(index, color);
  }

  style_color(const ImGuiCol index, const uint32 value)
  {
    ImGui::PushStyleColor(index, value);
  }

  ~style_color() { pop(); }

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

struct group final
{
  TACTILE_DEFAULT_COPY(group)
  TACTILE_DEFAULT_MOVE(group)

  group() { ImGui::BeginGroup(); }

  ~group() { ImGui::EndGroup(); }
};

class child final
{
 public:
  TACTILE_DEFAULT_COPY(child)
  TACTILE_DEFAULT_MOVE(child)

  explicit child(const char* id,
                 const ImVec2& size = {0, 0},
                 const bool border = false,
                 const ImGuiWindowFlags flags = 0)
      : mOpen{ImGui::BeginChild(id, size, border, flags)}
  {}

  ~child() { ImGui::EndChild(); }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class combo final
{
 public:
  TACTILE_DEFAULT_COPY(combo)
  TACTILE_DEFAULT_MOVE(combo)

  combo(const char* name, const char* current) : mOpen{ImGui::BeginCombo(name, current)}
  {}

  ~combo()
  {
    if (mOpen) {
      ImGui::EndCombo();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class tab_bar final
{
 public:
  TACTILE_DEFAULT_COPY(tab_bar)
  TACTILE_DEFAULT_MOVE(tab_bar)

  explicit tab_bar(const char* name, const ImGuiTabBarFlags flags = 0)
      : mOpen{ImGui::BeginTabBar(name, flags)}
  {}

  ~tab_bar()
  {
    if (mOpen) {
      ImGui::EndTabBar();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class tab_item final
{
 public:
  TACTILE_DEFAULT_COPY(tab_item)
  TACTILE_DEFAULT_MOVE(tab_item)

  explicit tab_item(const char* name,
                    bool* open = nullptr,
                    const ImGuiTabItemFlags flags = 0)
      : mOpen{ImGui::BeginTabItem(name, open, flags)}
  {}

  ~tab_item()
  {
    if (mOpen) {
      ImGui::EndTabItem();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class table final
{
 public:
  TACTILE_DEFAULT_COPY(table)
  TACTILE_DEFAULT_MOVE(table)

  table(const char* name, const int nColumns, const ImGuiTableFlags flags = 0)
      : mOpen{ImGui::BeginTable(name, nColumns, flags)}
  {}

  ~table()
  {
    if (mOpen) {
      ImGui::EndTable();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class popup final
{
 public:
  TACTILE_DEFAULT_COPY(popup)
  TACTILE_DEFAULT_MOVE(popup)

  explicit popup(const char* name, const ImGuiWindowFlags flags = 0)
      : mOpen{ImGui::BeginPopup(name, flags)}
  {}

  ~popup()
  {
    if (mOpen) {
      ImGui::EndPopup();
    }
  }

  [[nodiscard]] static auto for_item(const char* name, const ImGuiPopupFlags flags = 1)
      -> popup
  {
    return popup{ImGui::BeginPopupContextItem(name, flags)};
  }

  [[nodiscard]] static auto for_window(const char* name, const ImGuiPopupFlags flags = 1)
      -> popup
  {
    return popup{ImGui::BeginPopupContextWindow(name, flags)};
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};

  explicit popup(const bool open) : mOpen{open} {}
};

class list_box final
{
 public:
  TACTILE_DEFAULT_COPY(list_box)
  TACTILE_DEFAULT_MOVE(list_box)

  explicit list_box(const char* label, const ImVec2& size = {0, 0})
      : mOpen{ImGui::BeginListBox(label, size)}
  {}

  ~list_box()
  {
    if (mOpen) {
      ImGui::EndListBox();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class menu final
{
 public:
  TACTILE_DEFAULT_COPY(menu)
  TACTILE_DEFAULT_MOVE(menu)

  explicit menu(const char* name, const bool enabled = true)
      : mOpen{ImGui::BeginMenu(name, enabled)}
  {}

  ~menu()
  {
    if (mOpen) {
      ImGui::EndMenu();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class modal final
{
 public:
  TACTILE_DEFAULT_COPY(modal)
  TACTILE_DEFAULT_MOVE(modal)

  explicit modal(const char* name, const ImGuiWindowFlags flags = 0, bool* open = nullptr)
      : mOpen{ImGui::BeginPopupModal(name, open, flags)}
  {}

  ~modal()
  {
    if (mOpen) {
      ImGui::EndPopup();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class window final
{
 public:
  TACTILE_DEFAULT_COPY(window)
  TACTILE_DEFAULT_MOVE(window)

  explicit window(const char* label,
                  const ImGuiWindowFlags flags = 0,
                  bool* open = nullptr)
      : mOpen{ImGui::Begin(label, open, flags)}
  {}

  ~window() { ImGui::End(); }

  [[nodiscard]] auto has_focus(const ImGuiFocusedFlags flags = 0) const -> bool
  {
    return mOpen && ImGui::IsWindowFocused(flags);
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

  [[nodiscard]] static auto current_window_contains_mouse() -> bool
  {
    const auto min = ImGui::GetWindowContentRegionMin();
    const auto max = ImGui::GetWindowContentRegionMax();
    return ImGui::IsMouseHoveringRect(min, max);
  }

 private:
  bool mOpen{};
};

class tree_node final
{
 public:
  TACTILE_DEFAULT_COPY(tree_node)
  TACTILE_DEFAULT_MOVE(tree_node)

  explicit tree_node(const char* id, const ImGuiTreeNodeFlags flags = 0)
      : mOpen{ImGui::TreeNodeEx(id, flags)}
  {}

  template <typename... Args>
  explicit tree_node(const char* id,
                     const ImGuiTreeNodeFlags flags,
                     const char* fmt,
                     Args&&... args)
      : mOpen{ImGui::TreeNodeEx(id, flags, fmt, std::forward<Args>(args)...)}
  {}

  ~tree_node()
  {
    if (mOpen) {
      ImGui::TreePop();
    }
  }

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

}  // namespace tactile::scoped