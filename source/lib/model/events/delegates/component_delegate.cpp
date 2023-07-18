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

#include "component_delegate.hpp"

#include "cmd/component/add_component_attr.hpp"
#include "cmd/component/attach_component.hpp"
#include "cmd/component/define_component.hpp"
#include "cmd/component/detach_component.hpp"
#include "cmd/component/duplicate_component_attr.hpp"
#include "cmd/component/remove_component_attr.hpp"
#include "cmd/component/rename_component.hpp"
#include "cmd/component/rename_component_attr.hpp"
#include "cmd/component/reset_attached_component.hpp"
#include "cmd/component/set_component_attr_type.hpp"
#include "cmd/component/undef_component.hpp"
#include "cmd/component/update_attached_component.hpp"
#include "cmd/component/update_component.hpp"
#include "common/debug/assert.hpp"
#include "model/components/component_components.hpp"
#include "model/documents/command_system.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_show_component_editor(Registry& registry, const ShowComponentEditorEvent&)
{
  const auto document_entity = sys::get_active_document(registry);
  if (document_entity != kNullEntity) {
    const auto& document = registry.get<Document>(document_entity);
    const auto& component_set = registry.get<ComponentSet>(document.component_set);

    auto& widgets = registry.get<WidgetState>();
    widgets.component_editor_dialog.active_definition = kNullEntity;

    if (!component_set.definitions.empty()) {
      widgets.component_editor_dialog.active_definition =
          component_set.definitions.front();
    }

    widgets.component_editor_dialog.should_open = true;
  }
}

void on_show_new_comp_dialog(Registry& registry, const ShowNewCompDialogEvent&)
{
  auto& widgets = registry.get<WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.new_comp_dialog;

  dialog_state.name_buffer.clear();
  dialog_state.should_open = true;
}

void on_show_rename_comp_dialog(Registry& registry,
                                const ShowRenameCompDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));
  const auto& component = registry.get<Component>(event.definition);

  auto& widgets = registry.get<WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.rename_comp_dialog;

  dialog_state.definition = event.definition;
  dialog_state.old_component_name = component.name;
  dialog_state.component_name_buffer = component.name;
  dialog_state.should_open = true;
}

void on_show_new_comp_attr_dialog(Registry& registry,
                                  const ShowNewCompAttrDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  auto& widgets = registry.get<WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.new_comp_attr_dialog;

  dialog_state.definition_entity = event.definition;
  dialog_state.attribute_name_buffer.clear();
  dialog_state.should_open = true;
}

void on_show_rename_comp_attr_dialog(Registry& registry,
                                     const ShowRenameCompAttrDialogEvent& event)
{
  auto& widgets = registry.get<WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.rename_comp_attr_dialog;

  dialog_state.definition = event.definition;
  dialog_state.attribute_name = event.attr_name;
  dialog_state.attribute_name_buffer.clear();
  dialog_state.should_open = true;
}

void on_define_component(Registry& registry, const DefineComponentEvent& event)
{
  const auto document_entity = sys::get_active_document(registry);
  if (document_entity != kNullEntity) {
    const auto& document = registry.get<Document>(document_entity);
    sys::try_execute<cmd::DefineComponent>(registry, document.component_set, event.name);
  }
}

void on_undef_component(Registry& registry, const UndefComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  const auto document_entity = sys::get_active_document(registry);
  if (document_entity != kNullEntity) {
    const auto& document = registry.get<Document>(document_entity);
    const auto& component = registry.get<Component>(event.definition);
    sys::try_execute<cmd::UndefComponent>(registry,
                                          document.component_set,
                                          component.name);
  }
}

void on_rename_component(Registry& registry, const RenameComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::RenameComponent>(registry, event.definition, event.name);
}

void on_update_component(Registry& registry, const UpdateComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::UpdateComponent>(registry,
                                         event.definition,
                                         event.attr_name,
                                         event.value);
}

void on_add_component_attr(Registry& registry, const AddComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::AddComponentAttr>(registry, event.definition, event.attr_name);
}

void on_remove_component_attr(Registry& registry, const RemoveComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::RemoveComponentAttr>(registry, event.definition, event.attr_name);
}

void on_rename_component_attr(Registry& registry, const RenameComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::RenameComponentAttr>(registry,
                                             event.definition,
                                             event.current_name,
                                             event.updated_name);
}

void on_duplicate_component_attr(Registry& registry,
                                 const DuplicateComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::DuplicateComponentAttr>(registry,
                                                event.definition,
                                                event.attr_name);
}

void on_set_component_attr_type(Registry& registry,
                                const SetComponentAttrTypeEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::SetComponentAttrType>(registry,
                                              event.definition,
                                              event.attr_name,
                                              event.type);
}

void on_attach_component(Registry& registry, const AttachComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::AttachComponent>(registry, event.context, event.definition);
}

void on_detach_component(Registry& registry, const DetachComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));
  TACTILE_ASSERT(sys::is_component_entity(registry, event.definition));

  sys::try_execute<cmd::DetachComponent>(registry, event.context, event.definition);
}

void on_reset_attached_component(Registry& registry,
                                 const ResetAttachedComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_component_entity(registry, event.attached_component));

  sys::try_execute<cmd::ResetAttachedComponent>(registry, event.attached_component);
}

void on_update_attached_component(Registry& registry,
                                  const UpdateAttachedComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_component_entity(registry, event.attached_component));

  sys::try_execute<cmd::UpdateAttachedComponent>(registry,
                                                 event.attached_component,
                                                 event.attr_name,
                                                 event.value);
}

}  // namespace tactile
