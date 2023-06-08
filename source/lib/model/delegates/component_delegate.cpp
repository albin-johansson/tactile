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

#include "components/component.hpp"
#include "components/document.hpp"
#include "model/systems/document_system.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_open_component_editor(Model& model, const OpenComponentEditorEvent&)
{
  const auto document_entity = sys::get_active_document(model);
  if (document_entity != kNullEntity) {
    const auto& document = model.get<Document>(document_entity);
    const auto& component_set = model.get<ComponentSet>(document.component_set);

    auto& widgets = model.get<ui::WidgetState>();
    widgets.component_editor_dialog.active_component_def.reset();

    if (!component_set.definitions.empty()) {
      widgets.component_editor_dialog.active_component_def =
          component_set.definitions.front();
    }

    widgets.component_editor_dialog.should_open = true;
  }
}

}  // namespace tactile
