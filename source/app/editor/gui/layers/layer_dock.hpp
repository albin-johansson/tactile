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

#include "dialogs/rename_layer_dialog.hpp"
#include "editor/gui/dock_widget.hpp"
#include "tactile.hpp"

namespace tactile {

class document_model;

class layer_dock final : public dock_widget
{
 public:
  layer_dock();

  ~layer_dock() noexcept override;

  void show_rename_layer_dialog(layer_id id);

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void set_visible(bool visible) override;

  [[nodiscard]] auto is_visible() const -> bool override;

 private:
  struct layer_dock_data;
  std::unique_ptr<layer_dock_data> mData;

  void update_buttons(const document_model& model,
                      const entt::registry& registry,
                      entt::dispatcher& dispatcher);
};

}  // namespace tactile
