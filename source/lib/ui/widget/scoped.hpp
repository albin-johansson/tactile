/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile::ui {

struct Scope final {
  TACTILE_DELETE_COPY(Scope);
  TACTILE_DELETE_MOVE(Scope);

  explicit Scope(const void* ptr);
  explicit Scope(const char* str);
  explicit Scope(const UUID& id);
  explicit Scope(int id);

  ~Scope();
};

struct Disable final {
  TACTILE_DELETE_COPY(Disable);
  TACTILE_DELETE_MOVE(Disable);

  explicit Disable(bool disable = true);
  ~Disable();
};

struct Indent final {
  TACTILE_DELETE_COPY(Indent);
  TACTILE_DELETE_MOVE(Indent);

  Indent();
  ~Indent();
};

struct Tooltip final {
  TACTILE_DELETE_COPY(Tooltip);
  TACTILE_DELETE_MOVE(Tooltip);

  Tooltip();
  ~Tooltip();
};

class StyleVar final {
 public:
  TACTILE_DELETE_COPY(StyleVar);
  TACTILE_DELETE_MOVE(StyleVar);

  StyleVar(ImGuiStyleVar index, const ImVec2& value);
  StyleVar(ImGuiStyleVar index, float value);

  ~StyleVar();

  void pop();

 private:
  bool mPopped {};
};

class StyleColor final {
 public:
  TACTILE_DELETE_COPY(StyleColor);
  TACTILE_DELETE_MOVE(StyleColor);

  StyleColor(ImGuiCol index, const ImVec4& color);
  StyleColor(ImGuiCol index, uint32 value);

  ~StyleColor();

  void pop();

 private:
  bool mPopped {};
};

struct Group final {
  TACTILE_DELETE_COPY(Group);
  TACTILE_DELETE_MOVE(Group);

  Group();
  ~Group();
};

class Child final {
 public:
  TACTILE_DELETE_COPY(Child);
  TACTILE_DELETE_MOVE(Child);

  explicit Child(const char* id,
                 const ImVec2& size = {0, 0},
                 bool border = false,
                 ImGuiWindowFlags flags = 0);

  ~Child();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};
};

class Combo final {
 public:
  TACTILE_DELETE_COPY(Combo);
  TACTILE_DELETE_MOVE(Combo);

  Combo(const char* name, const char* current);

  ~Combo();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};
};

class TabBar final {
 public:
  TACTILE_DELETE_COPY(TabBar);
  TACTILE_DELETE_MOVE(TabBar);

  explicit TabBar(const char* name, ImGuiTabBarFlags flags = 0);

  ~TabBar();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};
};

class TabItem final {
 public:
  TACTILE_DELETE_COPY(TabItem);
  TACTILE_DELETE_MOVE(TabItem);

  explicit TabItem(const char* name, bool* open = nullptr, ImGuiTabItemFlags flags = 0);

  ~TabItem();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  StyleColor mTextColor;
  bool mOpen {};
};

class Table final {
 public:
  TACTILE_DELETE_COPY(Table);
  TACTILE_DELETE_MOVE(Table);

  Table(const char* name, int column_count, ImGuiTableFlags flags = 0);

  ~Table();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};
};

class Popup final {
 public:
  TACTILE_DELETE_COPY(Popup);
  TACTILE_DELETE_MOVE(Popup);

  explicit Popup(const char* name, ImGuiWindowFlags flags = 0);

  ~Popup();

  [[nodiscard]] static auto for_item(const char* name, ImGuiPopupFlags flags = 1)
      -> Popup;

  [[nodiscard]] static auto for_window(const char* name, ImGuiPopupFlags flags = 1)
      -> Popup;

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};

  explicit Popup(const bool open);
};

class ListBox final {
 public:
  TACTILE_DELETE_COPY(ListBox);
  TACTILE_DELETE_MOVE(ListBox);

  explicit ListBox(const char* label, const ImVec2& size = {0, 0});

  ~ListBox();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};
};

class Selectable final {
 public:
  Selectable() = delete;
  TACTILE_DELETE_COPY(Selectable);
  TACTILE_DELETE_MOVE(Selectable);

  static auto list_item(const char* label,
                        bool selected = false,
                        ImGuiSelectableFlags flags = 0,
                        const ImVec2& size = {0, 0}) -> bool;

  static auto property(const char* label,
                       bool selected = false,
                       ImGuiSelectableFlags flags = 0,
                       const ImVec2& size = {0, 0}) -> bool;
};

class Menu final {
 public:
  TACTILE_DELETE_COPY(Menu);
  TACTILE_DELETE_MOVE(Menu);

  explicit Menu(const char* name, bool enabled = true);

  ~Menu();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};
};

class Modal final {
 public:
  TACTILE_DELETE_COPY(Modal);
  TACTILE_DELETE_MOVE(Modal);

  explicit Modal(const char* name, ImGuiWindowFlags flags = 0, bool* open = nullptr);

  ~Modal();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen {};
};

struct WindowData final {
  bool has_focus   : 1 {};
  bool was_hovered : 1 {};
  bool is_hovered  : 1 {};
  bool is_docked   : 1 {};
  bool is_open     : 1 {};
};

class Window final {
  inline static HashMap<const char*, WindowData> mWindowData;

 public:
  TACTILE_DELETE_COPY(Window);
  TACTILE_DELETE_MOVE(Window);

  explicit Window(const char* label, ImGuiWindowFlags flags = 0, bool* open = nullptr);

  ~Window();

  [[nodiscard]] auto has_focus(ImGuiFocusedFlags flags = 0) const -> bool;

  [[nodiscard]] auto mouse_entered() const -> bool;

  [[nodiscard]] auto mouse_exited() const -> bool;

  [[nodiscard]] static auto contains_mouse() -> bool;

  [[nodiscard]] auto is_hovered() const -> bool;

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  StyleColor mTextColor;
  const char* mLabel {};
  bool mOpen {};
};

class TreeNode final {
 public:
  TACTILE_DELETE_COPY(TreeNode);
  TACTILE_DELETE_MOVE(TreeNode);

  explicit TreeNode(const char* id, ImGuiTreeNodeFlags flags = 0);

  TreeNode(const char* id, ImGuiTreeNodeFlags flags, const char* label);

  ~TreeNode();

  [[nodiscard]] auto is_open() const noexcept -> bool { return mOpen; }

 private:
  StyleColor mTextColor;
  bool mOpen {};
};

}  // namespace tactile::ui
