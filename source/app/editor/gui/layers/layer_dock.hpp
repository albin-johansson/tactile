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
#include "editor/gui/common/dock_widget.hpp"
#include "tactile.hpp"

namespace tactile {

class DocumentModel;

class LayerDock final : public ADockWidget
{
 public:
  TACTILE_DELETE_COPY(LayerDock)
  TACTILE_DEFAULT_MOVE(LayerDock)

  LayerDock();

  ~LayerDock() noexcept override;

  void show_rename_layer_dialog(layer_id id);

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;

  void set_visible(bool visible) override;

  [[nodiscard]] auto is_visible() const -> bool override;

 private:
  struct LayerDockData;
  std::unique_ptr<LayerDockData> mData;

  void update_buttons(const DocumentModel& model,
                      const entt::registry& registry,
                      entt::dispatcher& dispatcher);
};

}  // namespace tactile
