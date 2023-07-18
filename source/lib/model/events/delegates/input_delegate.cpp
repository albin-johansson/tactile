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

#include "input_delegate.hpp"

#include <imgui_internal.h>

#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/menu_events.hpp"
#include "model/viewports/viewport_components.hpp"
#include "ui/dock/editor/editor_dock.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/widget_state.hpp"

namespace tactile {
namespace {

void _on_mouse_wheel(Registry& registry,
                     Dispatcher& dispatcher,
                     const cen::mouse_wheel_event& event)
{
  // There doesn't seem to be a good way to handle mouse "wheel" events using the public
  // ImGui APIs. Otherwise, it would be nicer to keep this code closer to the actual
  // widgets.

  const auto document_entity = sys::get_active_document(registry);
  const auto& widgets = registry.get<WidgetState>();

  if (document_entity != kNullEntity && !ImGui::GetTopMostPopupModal()) {
    const auto& document = registry.get<Document>(document_entity);
    const auto& document_viewport = registry.get<Viewport>(document_entity);

    if (widgets.editor_dock.is_hovered) {
      on_mouse_wheel_event_in_central_viewport(document_entity,
                                               document_viewport,
                                               dispatcher,
                                               event);
    }
    else if (document.type == DocumentType::Map && widgets.tileset_dock.has_hover) {
      const auto& map_document = registry.get<MapDocument>(document_entity);
      if (map_document.active_tileset != kNullEntity) {
        on_mouse_wheel_event_in_tileset_dock(registry,
                                             map_document.active_tileset,
                                             event,
                                             dispatcher);
      }
    }
  }
}

}  // namespace

void on_event(Registry& registry, Dispatcher& dispatcher, const cen::event_handler& event)
{
  switch (event.type().value()) {
    case cen::event_type::mouse_wheel:
      _on_mouse_wheel(registry, dispatcher, event.get<cen::mouse_wheel_event>());
      break;

    default:
      break;
  }
}

}  // namespace tactile
