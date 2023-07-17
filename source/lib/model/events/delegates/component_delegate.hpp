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

#pragma once

#include "model/events/component_events.hpp"
#include "model/registry.hpp"

namespace tactile {

void on_show_component_editor(Registry& registry, const ShowComponentEditorEvent& event);

void on_show_new_comp_dialog(Registry& registry, const ShowNewCompDialogEvent& event);

void on_show_rename_comp_dialog(Registry& registry,
                                const ShowRenameCompDialogEvent& event);

void on_show_new_comp_attr_dialog(Registry& registry,
                                  const ShowNewCompAttrDialogEvent& event);

void on_show_rename_comp_attr_dialog(Registry& registry,
                                     const ShowRenameCompAttrDialogEvent& event);

void on_define_component(Registry& registry, const DefineComponentEvent& event);

void on_undef_component(Registry& registry, const UndefComponentEvent& event);

void on_rename_component(Registry& registry, const RenameComponentEvent& event);

void on_update_component(Registry& registry, const UpdateComponentEvent& event);

void on_add_component_attr(Registry& registry, const AddComponentAttrEvent& event);

void on_remove_component_attr(Registry& registry, const RemoveComponentAttrEvent& event);

void on_rename_component_attr(Registry& registry, const RenameComponentAttrEvent& event);

void on_duplicate_component_attr(Registry& registry,
                                 const DuplicateComponentAttrEvent& event);

void on_set_component_attr_type(Registry& registry,
                                const SetComponentAttrTypeEvent& event);

void on_attach_component(Registry& registry, const AttachComponentEvent& event);

void on_detach_component(Registry& registry, const DetachComponentEvent& event);

void on_reset_attached_component(Registry& registry,
                                 const ResetAttachedComponentEvent& event);

void on_update_attached_component(Registry& registry,
                                  const UpdateAttachedComponentEvent& event);

}  // namespace tactile
