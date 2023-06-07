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

#include "components/document.hpp"
#include "components/viewport.hpp"
#include "model/event/menu_events.hpp"
#include "model/systems/document_system.hpp"
#include "ui/dock/editor/editor_dock.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/widget_state.hpp"

namespace tactile {
namespace {

void _on_keyboard([[maybe_unused]] Dispatcher& dispatcher, cen::keyboard_event event)
{
  // We don't care about these modifiers, they are just noise.
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  // update_shortcuts(event, dispatcher);
}

void _on_mouse_wheel(Model& model,
                     Dispatcher& dispatcher,
                     const cen::mouse_wheel_event& event)
{
  // There doesn't seem to be a good way to handle mouse "wheel" events using the public
  // ImGui APIs. Otherwise, it would be nicer to keep this code closer to the actual
  // widgets.

  const auto document_entity = sys::get_active_document(model);
  const auto& widgets = model.get<ui::WidgetState>();

  if (document_entity != kNullEntity && !ImGui::GetTopMostPopupModal()) {
    const auto& document = model.get<Document>(document_entity);
    const auto& document_viewport = model.get<Viewport>(document_entity);

    if (widgets.editor_dock.is_hovered) {
      ui::viewport_widget_mouse_wheel_event_handler(document_entity,
                                                    document_viewport,
                                                    dispatcher,
                                                    event);
    }
    else if (document.type == DocumentType::Map && widgets.tileset_dock.has_hover) {
      const auto& map_document = model.get<MapDocument>(document_entity);
      // const auto& map = model.get<Map>(map_document.map);

      if (map_document.active_tileset != kNullEntity) {
        // const auto& attached_tileset =
        //     model.get<AttachedTileset>(map_document.active_tileset);

        // TODO ui::tileset_dock_mouse_wheel_event_handler(tileset_ref, event,
        // get_dispatcher());
      }
    }
  }
}

}  // namespace

void on_event(Model& model, Dispatcher& dispatcher, const cen::event_handler& event)
{
  switch (event.type().value()) {
    case cen::event_type::key_up:
      [[fallthrough]];
    case cen::event_type::key_down:
      _on_keyboard(dispatcher, event.get<cen::keyboard_event>());
      break;

    case cen::event_type::mouse_wheel:
      _on_mouse_wheel(model, dispatcher, event.get<cen::mouse_wheel_event>());
      break;

    default:
      break;
  }
}

}  // namespace tactile
