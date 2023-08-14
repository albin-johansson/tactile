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

#include "layer_delegate.hpp"

#include "cmd/layer/add_layer.hpp"
#include "cmd/layer/duplicate_layer.hpp"
#include "cmd/layer/move_layer_down.hpp"
#include "cmd/layer/move_layer_up.hpp"
#include "cmd/layer/remove_layer.hpp"
#include "cmd/layer/rename_layer.hpp"
#include "cmd/layer/set_layer_opacity.hpp"
#include "cmd/layer/set_layer_visible.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/command_system.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "model/maps/map_system.hpp"
#include "tactile/core/common/assert.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_show_rename_layer_dialog(Registry& registry,
                                 const ShowRenameLayerDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_layer_entity(registry, event.layer));
  const auto& layer_context = registry.get<Context>(event.layer);

  auto& widgets = registry.get<WidgetState>();
  auto& rename_dialog = widgets.layer_dock.rename_layer_dialog;

  rename_dialog.layer = event.layer;
  rename_dialog.old_name = layer_context.name;
  rename_dialog.name_buffer = layer_context.name;
  rename_dialog.should_open = true;
}

void on_create_layer(Registry& registry, const CreateLayerEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    sys::try_execute<cmd::AddLayer>(registry, document_entity, event.type);
  }
}

void on_remove_layer(Registry& registry, const RemoveLayerEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    sys::try_execute<cmd::RemoveLayer>(registry, document_entity, event.layer);
  }
}

void on_rename_layer(Registry& registry, const RenameLayerEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    sys::try_execute<cmd::RenameLayer>(registry, event.layer, event.name);
  }
}

void on_duplicate_layer(Registry& registry, const DuplicateLayerEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& map_document = registry.get<MapDocument>(document_entity);

    sys::try_execute<cmd::DuplicateLayer>(registry, map_document.map, event.layer);
  }
}

void on_select_layer(Registry& registry, const SelectLayerEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& map_document = registry.get<MapDocument>(document_entity);

    auto& map = registry.get<Map>(map_document.map);
    map.active_layer = event.layer;
  }
}

void on_move_layer_up(Registry& registry, const MoveLayerUpEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& map_document = registry.get<MapDocument>(document_entity);

    sys::try_execute<cmd::MoveLayerUp>(registry, map_document.map, event.layer);
  }
}

void on_move_layer_down(Registry& registry, const MoveLayerDownEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& map_document = registry.get<MapDocument>(document_entity);

    sys::try_execute<cmd::MoveLayerDown>(registry, map_document.map, event.layer);
  }
}

void on_set_layer_opacity(Registry& registry, const SetLayerOpacityEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    sys::try_execute<cmd::SetLayerOpacity>(registry, event.layer, event.opacity);
  }
}

void on_set_layer_visible(Registry& registry, const SetLayerVisibleEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    sys::try_execute<cmd::SetLayerVisible>(registry, event.layer, event.visible);
  }
}

}  // namespace tactile
