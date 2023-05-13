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

#include "add_layer_context_menu.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/layer_events.hpp"
#include "ui/style/icons.hpp"

namespace tactile::ui {

AddLayerContextMenu::AddLayerContextMenu()
    : ContextMenu {"AddLayerContextMenu"}
{
}

void AddLayerContextMenu::on_update(const Model&, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (ImGui::MenuItem(lang.action.tile_layer.c_str())) {
    dispatcher.enqueue<AddLayerEvent>(LayerType::TileLayer);
  }

  if (ImGui::MenuItem(lang.action.object_layer.c_str())) {
    dispatcher.enqueue<AddLayerEvent>(LayerType::ObjectLayer);
  }

  if (ImGui::MenuItem(lang.action.group_layer.c_str())) {
    dispatcher.enqueue<AddLayerEvent>(LayerType::GroupLayer);
  }
}

}  // namespace tactile::ui