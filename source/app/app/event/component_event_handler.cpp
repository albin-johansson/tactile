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

#include <entt/signal/dispatcher.hpp>

#include "app/app_context.hpp"
#include "app/event/event_handlers.hpp"
#include "model/document/document.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "ui/dock/comp/component_editor.hpp"

namespace tactile {
namespace {

void on_show_component_editor()
{
  ui::open_component_editor_dialog(get_model());
}

void on_define_component(const DefineComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->define_component(event.name);
  }
}

void on_undef_component(const UndefComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->undef_component(event.component_id);
  }
}

void on_rename_component(const RenameComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->rename_component(event.component_id, event.name);
  }
}

void on_update_component(const UpdateComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->update_component(event.component_id, event.attr_name, event.value);
  }
}

void on_attach_component(const AttachComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->attach_component(event.context_id, event.component_id);
  }
}

void on_detach_component(const DetachComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->detach_component(event.context_id, event.component_id);
  }
}

void on_update_attached_component(const UpdateAttachedComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->update_attached_component(event.context_id,
                                        event.component_id,
                                        event.attr_name,
                                        event.value);
  }
}

void on_reset_attached_component(const ResetAttachedComponentEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->reset_attached_component(event.context_id, event.component_id);
  }
}

void on_add_component_attr(const AddComponentAttrEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->add_component_attribute(event.component_id, event.attr_name);
  }
}

void on_remove_component_attr(const RemoveComponentAttrEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->remove_component_attribute(event.component_id, event.attr_name);
  }
}

void on_rename_component_attr(const RenameComponentAttrEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->rename_component_attribute(event.component_id,
                                         event.current_name,
                                         event.updated_name);
  }
}

void on_duplicate_component_attr(const DuplicateComponentAttrEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->duplicate_component_attribute(event.component_id, event.attr_name);
  }
}

void on_set_component_attr_type(const SetComponentAttrTypeEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->set_component_attribute_type(event.component_id,
                                           event.attr_name,
                                           event.type);
  }
}

}  // namespace

void install_component_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<OpenComponentEditorEvent>().connect<&on_show_component_editor>();

  dispatcher.sink<DefineComponentEvent>().connect<&on_define_component>();
  dispatcher.sink<UndefComponentEvent>().connect<&on_undef_component>();
  dispatcher.sink<RenameComponentEvent>().connect<&on_rename_component>();
  dispatcher.sink<UpdateComponentEvent>().connect<&on_update_component>();

  // clang-format off
  dispatcher.sink<AttachComponentEvent>().connect<&on_attach_component>();
  dispatcher.sink<DetachComponentEvent>().connect<&on_detach_component>();
  dispatcher.sink<UpdateAttachedComponentEvent>().connect<&on_update_attached_component>();
  dispatcher.sink<ResetAttachedComponentEvent>().connect<&on_reset_attached_component>();
  // clang-format on

  dispatcher.sink<AddComponentAttrEvent>().connect<&on_add_component_attr>();
  dispatcher.sink<RemoveComponentAttrEvent>().connect<&on_remove_component_attr>();
  dispatcher.sink<RenameComponentAttrEvent>().connect<&on_rename_component_attr>();
  dispatcher.sink<DuplicateComponentAttrEvent>().connect<&on_duplicate_component_attr>();
  dispatcher.sink<SetComponentAttrTypeEvent>().connect<&on_set_component_attr_type>();
}

}  // namespace tactile
