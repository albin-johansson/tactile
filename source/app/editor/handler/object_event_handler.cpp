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

#include "object_event_handler.hpp"

#include <entt/signal/dispatcher.hpp>

#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "model/document/map_document.hpp"
#include "model/event/object_events.hpp"
#include "model/model.hpp"
#include "ui/app_context.hpp"
#include "ui/viewport/map_viewport.hpp"

namespace tactile {
namespace {

void on_select_object(const SelectObjectEvent& event)
{
  if (auto* document = get_model().active_map()) {
    auto& layer = document->get_map().invisible_root().object_layer(event.layer_id);
    layer.select_object(event.object_id);
  }
}

void on_remove_object(const RemoveObjectEvent& event)
{
  if (auto* document = get_model().active_map()) {
    document->remove_object(event.object_id);
  }
}

void on_move_object(const MoveObjectEvent& event)
{
  if (auto* map = get_model().active_map()) {
    map->move_object(event.object_id, event.previous, event.updated);
  }
}

void on_set_object_name(const SetObjectNameEvent& event)
{
  if (auto* map = get_model().active_map()) {
    map->set_object_name(event.object_id, event.name);
  }
}

void on_set_object_tag(const SetObjectTagEvent& event)
{
  if (auto* document = get_model().active_map()) {
    document->set_object_tag(event.object_id, event.tag);
  }
}

void on_set_object_visible(const SetObjectVisibleEvent& event)
{
  if (auto* document = get_model().active_map()) {
    document->set_object_visible(event.object_id, event.visible);
  }
}

void on_spawn_object_context_menu(const SpawnObjectContextMenuEvent&)
{
  ui::open_object_context_menu();
}

}  // namespace

void install_object_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<SelectObjectEvent>().connect<&on_select_object>();
  dispatcher.sink<RemoveObjectEvent>().connect<&on_remove_object>();
  dispatcher.sink<MoveObjectEvent>().connect<&on_move_object>();
  dispatcher.sink<SetObjectNameEvent>().connect<&on_set_object_name>();
  dispatcher.sink<SetObjectTagEvent>().connect<&on_set_object_tag>();
  dispatcher.sink<SetObjectVisibleEvent>().connect<&on_set_object_visible>();

  dispatcher.sink<SpawnObjectContextMenuEvent>().connect<&on_spawn_object_context_menu>();
}

}  // namespace tactile
