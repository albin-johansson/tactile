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

#include "core/context/context_manager.hpp"
#include "handlers/event_handlers.hpp"
#include "model/document/document.hpp"
#include "model/event/property_events.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"
#include "ui/dock/property/dialogs/add_property_dialog.hpp"
#include "ui/dock/property/dialogs/change_property_type_dialog.hpp"
#include "ui/dock/property/dialogs/rename_property_dialog.hpp"

namespace tactile {
namespace {

void on_show_new_property_dialog()
{
  if (auto* document = get_model().active_document()) {
    const auto& context_id = document->get_contexts().active_context_id();
    ui::open_add_property_dialog(context_id);
  }
}

void on_show_rename_property_dialog(const ShowRenamePropertyDialogEvent& event)
{
  if (const auto* document = get_model().active_document()) {
    const auto& context_id = document->get_contexts().active_context_id();
    ui::open_rename_property_dialog(context_id, event.current_name);
  }
}

void on_show_change_property_type_dialog(const ShowChangePropertyTypeDialogEvent& event)
{
  if (const auto* document = get_model().active_document()) {
    const auto& context_id = document->get_contexts().active_context_id();
    ui::open_change_property_type_dialog(context_id, event.name, event.current_type);
  }
}

void on_add_property(const AddPropertyEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->add_property(event.context_id, event.name, event.type);
  }
}

void on_remove_property(const RemovePropertyEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->remove_property(event.context_id, event.name);
  }
}

void on_rename_property(const RenamePropertyEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->rename_property(event.context_id, event.old_name, event.new_name);
  }
}

void on_update_property(const UpdatePropertyEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->update_property(event.context_id, event.name, event.value);
  }
}

void on_change_property_type(const ChangePropertyTypeEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->change_property_type(event.context_id, event.name, event.type);
  }
}

void on_inspect_context(const InspectContextEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->get_contexts().select(event.context_id);
  }
}

}  // namespace

void install_property_event_handler()
{
  auto& dispatcher = get_dispatcher();

  // clang-format off
  // TODO rename event
  dispatcher.sink<ShowAddPropertyDialogEvent>().connect<&on_show_new_property_dialog>();
  dispatcher.sink<ShowRenamePropertyDialogEvent>().connect<&on_show_rename_property_dialog>();
  dispatcher.sink<ShowChangePropertyTypeDialogEvent>().connect<&on_show_change_property_type_dialog>();
  // clang-format on

  dispatcher.sink<AddPropertyEvent>().connect<&on_add_property>();
  dispatcher.sink<RemovePropertyEvent>().connect<&on_remove_property>();
  dispatcher.sink<RenamePropertyEvent>().connect<&on_rename_property>();
  dispatcher.sink<UpdatePropertyEvent>().connect<&on_update_property>();
  dispatcher.sink<ChangePropertyTypeEvent>().connect<&on_change_property_type>();

  dispatcher.sink<InspectContextEvent>().connect<&on_inspect_context>();
}

}  // namespace tactile
