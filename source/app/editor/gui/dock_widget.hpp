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

#include <entt/entt.hpp>
#include <imgui.h>

#include "tactile.hpp"

namespace tactile {

class document_model;

class dock_widget
{
 public:
  TACTILE_DEFAULT_COPY(dock_widget)
  TACTILE_DEFAULT_MOVE(dock_widget)

  dock_widget(const char* title, ImGuiWindowFlags flags);

  virtual ~dock_widget() noexcept = default;

  void update(const document_model& model, entt::dispatcher& dispatcher);

  [[nodiscard]] auto has_focus() const noexcept -> bool { return mHasFocus; }

 protected:
  virtual void on_update([[maybe_unused]] const document_model& model,
                         [[maybe_unused]] entt::dispatcher& dispatcher)
  {}

  void set_close_button_enabled(bool enabled);

  void set_focus_flags(ImGuiFocusedFlags flags);

  virtual void set_visible(bool visible) = 0;

  [[nodiscard]] virtual auto is_visible() const -> bool = 0;

 private:
  const char* mTitle{};
  ImGuiWindowFlags mWindowFlags{};
  ImGuiFocusedFlags mFocusFlags{};
  bool mHasFocus{};
  bool mHasCloseButton{};
};

}  // namespace tactile