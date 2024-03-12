// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

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

void AddLayerContextMenu::on_update(const DocumentModel&, entt::dispatcher& dispatcher)
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
