// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <entt/signal/dispatcher.hpp>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include "common/util/fmt.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "handlers/event_handlers.hpp"
#include "model/document/map_document.hpp"
#include "model/event/object_events.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"
#include "ui/viewport/map_viewport.hpp"

namespace tactile {
namespace {

void on_select_object(const SelectObjectEvent& event)
{
  spdlog::trace("SelectObjectEvent(layer_id: {}, object_id: {})",
                event.layer_id,
                event.object_id);

  if (auto* map_document = get_model().active_map_document()) {
    auto& root = map_document->get_map().get_invisible_root();
    auto& layer = root.get_object_layer(event.layer_id);
    layer.select_object(event.object_id);
  }
}

void on_remove_object(const RemoveObjectEvent& event)
{
  spdlog::trace("RemoveObjectEvent(object_id: {})", event.object_id);

  if (auto* map_document = get_model().active_map_document()) {
    map_document->remove_object(event.object_id);
  }
}

void on_move_object(const MoveObjectEvent& event)
{
  spdlog::trace("MoveObjectEvent(object_id: {}, previous: {}, updated: {})",
                event.object_id,
                event.previous,
                event.updated);

  if (auto* map_document = get_model().active_map_document()) {
    map_document->move_object(event.object_id, event.previous, event.updated);
  }
}

void on_set_object_name(const SetObjectNameEvent& event)
{
  if (auto* map_document = get_model().active_map_document()) {
    map_document->set_object_name(event.object_id, event.name);
  }
}

void on_set_object_tag(const SetObjectTagEvent& event)
{
  if (auto* map_document = get_model().active_map_document()) {
    map_document->set_object_tag(event.object_id, event.tag);
  }
}

void on_set_object_visible(const SetObjectVisibleEvent& event)
{
  if (auto* map_document = get_model().active_map_document()) {
    map_document->set_object_visible(event.object_id, event.visible);
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
