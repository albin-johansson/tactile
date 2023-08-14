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

#include "property_delegate.hpp"

#include "cmd/property/create_property.hpp"
#include "cmd/property/remove_property.hpp"
#include "cmd/property/rename_property.hpp"
#include "cmd/property/set_property_type.hpp"
#include "cmd/property/update_property.hpp"
#include "common/util/lookup.hpp"
#include "core/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/command_system.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_show_new_property_dialog(Registry& registry,
                                 const ShowNewPropertyDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));

  auto& widgets = registry.get<WidgetState>();
  auto& new_property_dialog = widgets.property_dock.new_property_dialog;

  new_property_dialog.context = event.context;
  new_property_dialog.name_buffer.clear();
  new_property_dialog.property_type = PropertyType::Str;
  new_property_dialog.should_open = true;
}

void on_show_rename_property_dialog(Registry& registry,
                                    const ShowRenamePropertyDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));

  auto& widgets = registry.get<WidgetState>();
  auto& rename_property_dialog = widgets.property_dock.rename_property_dialog;

  rename_property_dialog.context = event.context;
  rename_property_dialog.old_name = event.property_name;
  rename_property_dialog.name_buffer.clear();
  rename_property_dialog.should_open = true;
}

void on_show_set_property_type_dialog(Registry& registry,
                                      const ShowSetPropertyTypeDialogEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));

  auto& widgets = registry.get<WidgetState>();
  auto& set_property_type_dialog = widgets.property_dock.set_property_type_dialog;

  const auto& context = registry.get<Context>(event.context);
  const auto property_type = lookup_in(context.props, event.property_name).get_type();

  set_property_type_dialog.context = event.context;
  set_property_type_dialog.property_name = event.property_name;
  set_property_type_dialog.old_type = property_type;
  set_property_type_dialog.current_type = property_type;
  set_property_type_dialog.should_open = true;
}

void on_inspect_context(Registry& registry, const InspectContextEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));

  const auto document_entity = sys::get_active_document(registry);
  if (document_entity != kNullEntity) {
    auto& document = registry.get<Document>(document_entity);
    document.active_context = event.context;
  }
}

void on_create_property(Registry& registry, const CreatePropertyEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));
  sys::try_execute<cmd::CreateProperty>(registry, event.context, event.name, event.type);
}

void on_remove_property(Registry& registry, const RemovePropertyEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));
  sys::try_execute<cmd::RemoveProperty>(registry, event.context, event.name);
}

void on_rename_property(Registry& registry, const RenamePropertyEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));
  sys::try_execute<cmd::RenameProperty>(registry,
                                        event.context,
                                        event.old_name,
                                        event.new_name);
}

void on_update_property(Registry& registry, const UpdatePropertyEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));
  sys::try_execute<cmd::UpdateProperty>(registry, event.context, event.name, event.value);
}

void on_set_property_type(Registry& registry, const SetPropertyTypeEvent& event)
{
  TACTILE_ASSERT(sys::is_context_entity(registry, event.context));
  sys::try_execute<cmd::SetPropertyType>(registry, event.context, event.name, event.type);
}

}  // namespace tactile
