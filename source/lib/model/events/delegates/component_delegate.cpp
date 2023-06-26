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

void on_show_component_editor(Model& model, const ShowComponentEditorEvent&)
{
  const auto document_entity = sys::get_active_document(model);
  if (document_entity != kNullEntity) {
    const auto& document = model.get<Document>(document_entity);
    const auto& component_set = model.get<ComponentSet>(document.component_set);

    auto& widgets = model.get<ui::WidgetState>();
    widgets.component_editor_dialog.active_definition = kNullEntity;

    if (!component_set.definitions.empty()) {
      widgets.component_editor_dialog.active_definition =
          component_set.definitions.front();
    }

    widgets.component_editor_dialog.should_open = true;
  }
}

void on_show_new_comp_dialog(Model& model, const ShowNewCompDialogEvent&)
{
  auto& widgets = model.get<ui::WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.new_comp_dialog;

  dialog_state.name_buffer.clear();
  dialog_state.should_open = true;
}

void on_show_rename_comp_dialog(Model& model, const ShowRenameCompDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));
  const auto& component = model.get<Component>(event.definition);

  auto& widgets = model.get<ui::WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.rename_comp_dialog;

  dialog_state.definition = event.definition;
  dialog_state.old_component_name = component.name;
  dialog_state.component_name_buffer = component.name;
  dialog_state.should_open = true;
}

void on_show_new_comp_attr_dialog(Model& model, const ShowNewCompAttrDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  auto& widgets = model.get<ui::WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.new_comp_attr_dialog;

  dialog_state.definition_entity = event.definition;
  dialog_state.attribute_name_buffer.clear();
  dialog_state.should_open = true;
}

void on_show_rename_comp_attr_dialog(Model& model,
                                     const ShowRenameCompAttrDialogEvent& event)
{
  auto& widgets = model.get<ui::WidgetState>();
  auto& dialog_state = widgets.component_editor_dialog.rename_comp_attr_dialog;

  dialog_state.definition = event.definition;
  dialog_state.attribute_name = event.attr_name;
  dialog_state.attribute_name_buffer.clear();
  dialog_state.should_open = true;
}

void on_define_component(Model& model, const DefineComponentEvent& event)
{
  const auto document_entity = sys::get_active_document(model);
  if (document_entity != kNullEntity) {
    const auto& document = model.get<Document>(document_entity);
    sys::try_execute<cmd::DefineComponent>(model, document.component_set, event.name);
  }
}

void on_undef_component(Model& model, const UndefComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  const auto document_entity = sys::get_active_document(model);
  if (document_entity != kNullEntity) {
    const auto& document = model.get<Document>(document_entity);
    const auto& component = model.get<Component>(event.definition);
    sys::try_execute<cmd::UndefComponent>(model, document.component_set, component.name);
  }
}

void on_rename_component(Model& model, const RenameComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::RenameComponent>(model, event.definition, event.name);
}

void on_update_component(Model& model, const UpdateComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::UpdateComponent>(model,
                                         event.definition,
                                         event.attr_name,
                                         event.value);
}

void on_add_component_attr(Model& model, const AddComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::AddComponentAttr>(model, event.definition, event.attr_name);
}

void on_remove_component_attr(Model& model, const RemoveComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::RemoveComponentAttr>(model, event.definition, event.attr_name);
}

void on_rename_component_attr(Model& model, const RenameComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::RenameComponentAttr>(model,
                                             event.definition,
                                             event.current_name,
                                             event.updated_name);
}

void on_duplicate_component_attr(Model& model, const DuplicateComponentAttrEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::DuplicateComponentAttr>(model, event.definition, event.attr_name);
}

void on_set_component_attr_type(Model& model, const SetComponentAttrTypeEvent& event)
{
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::SetComponentAttrType>(model,
                                              event.definition,
                                              event.attr_name,
                                              event.type);
}

void on_attach_component(Model& model, const AttachComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(model, event.context));
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::AttachComponent>(model, event.context, event.definition);
}

void on_detach_component(Model& model, const DetachComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(model, event.context));
  TACTILE_ASSERT(sys::is_component_entity(model, event.definition));

  sys::try_execute<cmd::DetachComponent>(model, event.context, event.definition);
}

void on_reset_attached_component(Model& model, const ResetAttachedComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_component_entity(model, event.attached_component));

  sys::try_execute<cmd::ResetAttachedComponent>(model, event.attached_component);
}

void on_update_attached_component(Model& model, const UpdateAttachedComponentEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_component_entity(model, event.attached_component));

  sys::try_execute<cmd::UpdateAttachedComponent>(model,
                                                 event.attached_component,
                                                 event.attr_name,
                                                 event.value);
}

}  // namespace tactile
