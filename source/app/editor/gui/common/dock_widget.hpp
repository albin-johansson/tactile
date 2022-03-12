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

#include <memory>  // unique_ptr

#include <entt/entt.hpp>
#include <imgui.h>

#include "tactile.hpp"

namespace tactile {

class DocumentModel;

class ADockWidget
{
 public:
  TACTILE_DELETE_COPY(ADockWidget)
  TACTILE_DEFAULT_MOVE(ADockWidget)

  ADockWidget(const char* title, ImGuiWindowFlags flags);

  virtual ~ADockWidget() noexcept;

  void update(const DocumentModel& model, entt::dispatcher& dispatcher);

  [[nodiscard]] auto has_focus() const -> bool;

  [[nodiscard]] auto has_mouse_hover() const  -> bool;

 protected:
  virtual void on_update([[maybe_unused]] const DocumentModel& model,
                         [[maybe_unused]] entt::dispatcher& dispatcher)
  {}

  void set_close_button_enabled(bool enabled);

  void set_focus_flags(ImGuiFocusedFlags flags);

  virtual void set_visible(bool visible) = 0;

  [[nodiscard]] virtual auto is_visible() const -> bool = 0;

 private:
  struct Data;
  std::unique_ptr<Data> mData;
};

}  // namespace tactile