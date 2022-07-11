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

#include "ui.hpp"

#include "core/model.hpp"
#include "editor/ui/components/component_dock.hpp"
#include "editor/ui/dock_space.hpp"
#include "editor/ui/layers/layer_dock.hpp"
#include "editor/ui/log/log_dock.hpp"
#include "editor/ui/menus/menu_bar.hpp"
#include "editor/ui/properties/property_dock.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"
#include "editor/ui/tilesets/tileset_dock.hpp"
#include "editor/ui/viewport/map_viewport_toolbar.hpp"
#include "editor/ui/viewport/viewport_widget.hpp"

namespace tactile::ui {

void update_widgets(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  update_menu_bar(model, dispatcher);
  update_dock_space();

  const auto* document = model.active_document();

  if (document) {
    if (document->is_map()) {
      update_layer_dock(model, dispatcher);
      update_tileset_dock(model, dispatcher);
    }

    update_property_dock(model, dispatcher);
    update_component_dock(model, dispatcher);
    update_log_dock();
  }

  update_viewport_widget(model, dispatcher);

  auto& dialogs = get_dialogs();
  dialogs.resize_map.update(model, dispatcher);
  dialogs.map_parse_error.update(model, dispatcher);
}

auto is_editor_focused() -> bool
{
  return is_map_toolbar_focused() || is_viewport_focused() || is_layer_dock_focused() ||
         is_tileset_dock_focused() || is_property_dock_focused() || is_log_dock_focused();
}

}  // namespace tactile::ui