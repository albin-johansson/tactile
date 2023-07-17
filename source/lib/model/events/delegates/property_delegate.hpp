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

#pragma once

#include "model/events/property_events.hpp"
#include "model/registry.hpp"

namespace tactile {

void on_show_new_property_dialog(Registry& registry,
                                 const ShowNewPropertyDialogEvent& event);

void on_show_rename_property_dialog(Registry& registry,
                                    const ShowRenamePropertyDialogEvent& event);

void on_show_set_property_type_dialog(Registry& registry,
                                      const ShowSetPropertyTypeDialogEvent& event);

void on_inspect_context(Registry& registry, const InspectContextEvent& event);

void on_create_property(Registry& registry, const CreatePropertyEvent& event);

void on_remove_property(Registry& registry, const RemovePropertyEvent& event);

void on_rename_property(Registry& registry, const RenamePropertyEvent& event);

void on_update_property(Registry& registry, const UpdatePropertyEvent& event);

void on_set_property_type(Registry& registry, const SetPropertyTypeEvent& event);

}  // namespace tactile
