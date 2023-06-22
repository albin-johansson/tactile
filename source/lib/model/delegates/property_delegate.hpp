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
#include "model/model.hpp"

namespace tactile {

void on_show_new_property_dialog(Model& model, const ShowNewPropertyDialogEvent& event);

void on_show_rename_property_dialog(Model& model,
                                    const ShowRenamePropertyDialogEvent& event);

void on_show_set_property_type_dialog(Model& model,
                                      const ShowSetPropertyTypeDialogEvent& event);

void on_inspect_context(Model& model, const InspectContextEvent& event);

void on_create_property(Model& model, const CreatePropertyEvent& event);

void on_remove_property(Model& model, const RemovePropertyEvent& event);

void on_rename_property(Model& model, const RenamePropertyEvent& event);

void on_update_property(Model& model, const UpdatePropertyEvent& event);

void on_set_property_type(Model& model, const SetPropertyTypeEvent& event);

}  // namespace tactile
